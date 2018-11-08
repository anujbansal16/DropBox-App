from flask import Flask
import sqlite3
from flask_sqlalchemy import SQLAlchemy
# Creates the application object 
app = Flask(__name__)
app.secret_key="qoadasmadadsakjadsadjk"

app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///test.db'
db=SQLAlchemy(app)
from app import models
# Import Views from the app module. (DO NOT Confuse with app variable)
from app import views

# Import is at the end to avoid circular reference