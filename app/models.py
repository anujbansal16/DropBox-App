from datetime import datetime
from app import db
from sqlalchemy import desc

class Users(db.Model):
    name = db.Column(db.String(50))
    username = db.Column(db.String(50), primary_key=True)
    password = db.Column(db.String(64), nullable=False)
    email = db.Column(db.String(60), nullable=False)
    files=db.relationship('Files',backref='users',lazy='subquery',order_by=(desc("Files.isFolder")))
    def __init__(self,name,username,password,email):
    	self.name=name
    	self.username=username
    	self.password=password
    	self.email=email

class Files(db.Model):
	name=db.Column(db.String(50),primary_key=True)		
	username = db.Column(db.String(50),db.ForeignKey('users.username'),nullable=False,primary_key=True)
	parent=db.Column(db.String(200),primary_key=True)
	size=db.Column(db.Integer)
	isFolder=db.Column(db.Boolean,nullable=False)
	isPublic=db.Column(db.Boolean,nullable=False)


# class Post(db.Model):
#     id = db.Column(db.Integer, primary_key=True)
#     title = db.Column(db.String(100), nullable=False)
#     date_posted = db.Column(db.DateTime, nullable=False, default=datetime.utcnow)
#     content = db.Column(db.Text, nullable=False)
#     username = db.Column(db.String, db.ForeignKey('users.username'), nullable=False)

#     def __repr__(self):
#         return "Post('{self.title}', '{self.date_posted}')"

db.create_all()


