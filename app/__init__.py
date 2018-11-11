from flask import Flask
import sqlite3
from flask_sqlalchemy import SQLAlchemy
import os
# Creates the application object 
app = Flask(__name__)
app.secret_key="qoadasmadadsakjadsadjk"
APP_ROOT = os.path.dirname(os.path.abspath(__file__))
uploadFolder="files"
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///test.db'
db=SQLAlchemy(app)
from app import models
# Import Views from the app module. (DO NOT Confuse with app variable)
from app import views

# Import is at the end to avoid circular reference