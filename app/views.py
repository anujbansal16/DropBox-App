from flask import render_template
from sqlalchemy import and_
import hashlib 
from flask import url_for, redirect, request, make_response,flash
# Importing Session Object to use Sessions
# from flask import session
from app.models import Users
from app import app
import utility
from app import db
from flask import jsonify

@app.route('/index')
@app.route('/')
def index():
	return render_template('index.html',title='Home',user="user",posts="posts")

@app.route("/register")
def register():
	return render_template('register.html')

@app.route("/submit/<action>",methods=['POST'])
def submit(action):
	if request.method=="POST":
		if action=="signin":
			if request.form["username"] and request.form["password"]:
				if Users.query.filter(and_(Users.username==request.form["username"],Users.password==hashlib.sha256(request.form["password"].encode()).hexdigest())).first():
					global user
					user=Users.query.filter(and_(Users.username==request.form["username"],Users.password==hashlib.sha256(request.form["password"].encode()).hexdigest())).first()
					return redirect(url_for("home"))
			return render_template("index.html",error=True,msg="Invalid userid or password",color="red")
		elif action=="register":
			return register(request)
		return render_template('error404.html')

@app.route("/home")
def home():
	return render_template("home.html",myfiles=(user.files),user=user)

@app.route("/createFolder",methods=['POST'])
def createFolder():
	return 'anuj'
	# return jsonify({'data': render_template('base.html', myfiles=user.files)})
	

	

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