import os
import time
from flask import render_template
from sqlalchemy import and_
from sqlalchemy import desc
import hashlib 
from flask import url_for, redirect, request, make_response,flash
from app.models import Users,Files
from app import app,APP_ROOT,uploadFolder
import utility
from app import db
from flask import jsonify
from flask import session
from flask import send_from_directory

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    return response


def isLoggedIn():
	if session.get('username'):
		return True
	return False

def getCurrentUser():
	return Users.query.filter_by(username=session["username"]).first()

def getCurrentUserFiles():
	files=Files.query.filter(and_(Files.username==session["username"],Files.parent==session["path"])).order_by(Files.name).all()
	return jsonify({'data': render_template('files.html', myfiles=files),'path':session["path"]})

def searchFiles(fileName):
	
	files=Files.query.filter(and_(Files.isPublic==True,Files.isFolder==False,Files.name.like("%"+fileName+"%"))).order_by(Files.name).all()
	filesPrv=Files.query.filter(and_(Files.username==session["username"],Files.isPublic==False,Files.name.like("%"+fileName+"%"))).order_by(Files.name).all()
	files=files+filesPrv	
	if files:
		print(files)
		return jsonify({'data': render_template('files.html', myfiles=files),'path':session["path"]})
	else:
		return jsonify({'data': "<h1>No results found for "+fileName+"</h1>"})


@app.route('/index')
@app.route('/')
def index():
	if isLoggedIn():
		return redirect(url_for("home"))
	return render_template('index.html')

@app.route('/logout')
def logout():
	session.pop('username', None)
   	return redirect(url_for('index'))
	

@app.route("/register")
def register():
	return render_template('register.html')

@app.route("/submit/<action>",methods=['POST'])
def submit(action):
	if request.method=="POST":
		if action=="signin":
			if request.form["username"] and request.form["password"]:
				if Users.query.filter(and_(Users.username==request.form["username"],Users.password==hashlib.sha256(request.form["password"].encode()).hexdigest())).first():
					# global user
					session["username"]=request.form["username"]
					session["path"]="/"
					user=Users.query.filter(and_(Users.username==request.form["username"],Users.password==hashlib.sha256(request.form["password"].encode()).hexdigest())).first()
					if user.totalSize:
						session["totalSize"]=humanReadableSize(user.totalSize);
					else:
						session["totalSize"]=humanReadableSize(0)
					return redirect(url_for("home"))
			return render_template("index.html",error=True,msg="Invalid userid or password",color="red")
		elif action=="register":
			return register(request)
		return render_template('error404.html')

@app.route("/home")
def home():
	if isLoggedIn():
		# user=Users.query.filter_by(username=session["username"]).first()
		session["path"]="/"
		# session["totalSize"]="anuj"
		files=Files.query.filter(and_(Files.username==session["username"],Files.parent==session["path"])).order_by(Files.name)
		return render_template("home.html",myfiles=files)
	else:
		return redirect(url_for('index'))



@app.route("/createFolder",methods=['POST'])
def createFolder():
	if isLoggedIn():
		fName=request.form["name"].strip()
		if not(fName):
			return jsonify({'data':"Please Provide folder name",'error':True})
		if "/" in fName:
			return jsonify({'data':"Folder Name doesn't include /",'error':True})

		folder=Files(fName,session['username'],session['path'],"",1,1)
		try:
			db.session.add(folder)
			db.session.commit()	
		except Exception as e:
			if 'IntegrityError' in str(e):
				print(str(e))
				return jsonify({'data':"Folder already exist",'error':True})
			return jsonify({'data',str(e)})
		return getCurrentUserFiles()
	return redirect(url_for('index'))




@app.route("/delete_data",methods=['POST'])
def delete_data():
	name = request.form['name']
	parent = request.form['parentPath']
	username = session['username']
	data = Files.query.filter(and_(Files.name == name, Files.username == username, Files.parent == parent)).first()
	user = Users.query.filter(Users.username == username).first()

	if data.isFolder == 0:
		print(APP_ROOT)
		try:
			user.totalSize -= data.size
			session["totalSize"]=humanReadableSize(user.totalSize)
			db.session.delete(data)
			db.session.commit()
			os.remove(APP_ROOT + "/" + uploadFolder + "/" + data.nameWithTS)

		except:
			print("Data not fount" + name + parent + username)
	else:
		data1 = Files.query.filter(and_(Files.username == username,Files.parent == (parent + name + "/"))).first()
		if data1 is None:
			try:
				db.session.delete(data)
				db.session.commit()
			except:
				return jsonify({'data':'Something went wrong','error':True})
		else:
			return jsonify({'data':"Folder is not empty ",'error':True})

	return getCurrentUserFiles()


@app.route("/openFolder",methods=['POST'])
def openFolder():
	if isLoggedIn():
		folderName=request.form["folderName"].strip()
		if folderName:
			session["path"]=request.form["parentPath"].strip()+folderName+"/"
			print(session["path"])
			# session["path"]=session["path"]+folderName+"/"
		return getCurrentUserFiles()
	return jsonify({'data':"Something went wrong",'error':True})		

@app.route("/goBack",methods=['GET'])
def goBack():
	if isLoggedIn:
		path=session["path"]
		if path!="/":
			path=path[0:len(path)-1]
			session["path"]=path[0:path.rfind("/")+1]
			print(session["path"])
			return getCurrentUserFiles()

	return jsonify({'data':"Something went wrong",'error':True})

@app.route("/search",methods=['GET'])
def search():
	if isLoggedIn():
		fName=request.args.get("name").strip();
		if not(fName):
			return jsonify({'data':"Please Provide some name",'error':True})
		else:
			return searchFiles(fName)
	return jsonify({'data':"Something went wrong",'error':True})	

@app.route("/changeMe",methods=['GET'])
def changeMe():
	# print("--------------adsd")
	fname=request.args.get("fname").strip()
	if fname:
		parentPath=request.args.get("parentPath").strip()
		myfile=Files.query.filter(and_(Files.name==fname,Files.username==session["username"],Files.parent==parentPath)).first()
		if myfile:
			try:
				if myfile.isPublic:
					myfile.isPublic=False
				else:
					myfile.isPublic=True
				db.session.commit()
			except Exception as e:
				return jsonify({'data':str(e),'error':True})
	return jsonify({'data':'success','error':False})

@app.route("/download", methods=['GET'])
def download():
	if isLoggedIn():
		fname=request.args.get("fname").strip()
		if fname:
			parentPath=request.args.get("parentPath").strip()
			owner=request.args.get("owner").strip()
			print("--------------------")
			print(fname+" "+owner+" "+parentPath)
			if owner==session["username"]:
				myfile=Files.query.filter(and_(Files.name==fname,Files.username==owner,Files.parent==parentPath)).first()
			else:
				myfile=Files.query.filter(and_(Files.name==fname,Files.username==owner,Files.isPublic==True,Files.parent==parentPath)).first()
			if myfile:
				uploads = os.path.join(APP_ROOT+"/",uploadFolder+"/")
				return send_from_directory(directory=uploads, filename=myfile.nameWithTS, as_attachment=True)
	return redirect(url_for('index'))
	
@app.route("/upload",methods=['POST'])
def upload():
	if isLoggedIn:
		if request.method =='POST':
			for key, file in request.files.items():
				currentT=time.time()
				currentT=str(currentT)
				oFileName=file.filename
				filename =currentT+file.filename
				fileD=Files(oFileName,session['username'],session['path'],0,0,0,filename,"0B")
				print("-----------------------------")
				print(filename)	
				try:
					db.session.add(fileD)
					db.session.commit()
					file.save(os.path.join(APP_ROOT+"/"+uploadFolder+"/", filename))
					fileD.size=os.stat(os.path.join(APP_ROOT+"/"+uploadFolder+"/", filename)).st_size
					fileD.humanReadableSize=humanReadableSize(fileD.size)
					if not fileD.users.totalSize:
						fileD.users.totalSize=0	
					fileD.users.totalSize+=fileD.size
					session["totalSize"]=humanReadableSize(fileD.users.totalSize)
					db.session.commit()
				except Exception as e:
					if 'IntegrityError' in str(e):
						return jsonify({'data':"File already exist (delete exising file first)",'error':True})
					return jsonify({'data':str(e),'error':True})
		return getCurrentUserFiles()
	return jsonify({'data':"Error:Something went wrong",'error':True})	
	

def register(request):
	name=request.form["name"].strip()
	email=request.form["email"].strip()
	username=request.form["username"].strip()
	password=request.form["password"].strip()
	repeat_pass=request.form["repeat-pass"].strip()
	if not(name and email and username and password and repeat_pass):
		return render_template('register.html',error=True,msg="Error: Fill all the details",color="red")
	elif password!=repeat_pass:
		return render_template('register.html',error=True,msg="Error: Both password doesn't matches.",color="red")
	
	if Users.query.filter_by(username=username).first():
		return render_template('register.html',error=True,msg="Error: User already exist with id "+username, color="red")
	else:
		password=hashlib.sha256(password.encode()).hexdigest()
		user=Users(name,username,password,email)
		db.session.add(user)	
		db.session.commit()
		return render_template('register.html',error=True,msg="Successfully registered",color="green")


@app.errorhandler(404)
def http_404_handler(error):
	return render_template('error404.html')

def humanReadableSize(var):
	unit ="B"
	if var > 1000:
		var = var/1000.0
		unit = "KB"
	if var > 1000:
		var = var/1000.0
		unit = "MB"
	if var > 1000:
		var = var/1000.0
		unit = "GB"
	return str(round(var,2)) + ' ' + unit
