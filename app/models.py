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
    def __init__(self,name,username,parent,size,isFolder,isPublic):
        self.name=name
        self.username=username
        self.parent=parent
        self.size=size
        self.isFolder=isFolder
        self.isPublic=isPublic

db.create_all()


