#!/usr/bin/python
# -*- coding: utf-8 -*-
from datetime import datetime
from app import db
from sqlalchemy import desc


class Users(db.Model):

    name = db.Column(db.String(50))
    username = db.Column(db.String(50), primary_key=True)
    password = db.Column(db.String(64), nullable=False)
    email = db.Column(db.String(60), nullable=False)
    files = db.relationship('Files', backref='users', lazy='subquery',
                            order_by=desc('Files.isFolder'))
    totalSize = db.Column(db.Integer, nullable=True)

    def __init__(
        self,
        name,
        username,
        password,
        email,
        totalSize=None,
        ):
        self.name = name
        self.username = username
        self.password = password
        self.email = email
        self.totalSize = totalSize


class Files(db.Model):

    name = db.Column(db.String(50), primary_key=True)
    username = db.Column(db.String(50), db.ForeignKey('users.username'
                         ), nullable=False, primary_key=True)
    parent = db.Column(db.String(200), primary_key=True)
    nameWithTS = db.Column(db.String(200), nullable=True)
    size = db.Column(db.Integer)
    isFolder = db.Column(db.Boolean, nullable=False)
    isPublic = db.Column(db.Boolean, nullable=False)
    humanReadableSize = db.Column(db.String(10), nullable=True)

    def __init__(
        self,
        name,
        username,
        parent,
        size,
        isFolder,
        isPublic,
        nameWithTS=None,
        humanReadableSize=None,
        ):
        self.name = name
        self.username = username
        self.parent = parent
        self.size = size
        self.isFolder = isFolder
        self.isPublic = isPublic
        self.nameWithTS = nameWithTS
        self.humanReadableSize = humanReadableSize


db.create_all()
