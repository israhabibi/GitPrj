import sqlite3
from flask import Flask, request, jsonify
# from flask_cors import CORS


def connect_to_db():
    conn = sqlite3.connect('GitPrj/Python/FlaskApi/database.db')
    return conn

def create_db_table():
    try:
        conn = connect_to_db()
        conn.execute('''DROP TABLE users''')
        conn.execute('''
            CREATE TABLE MonitorCuaca (
                Date INTEGER PRIMARY KEY NOT NULL,
                name TEXT NOT NULL,
                email TEXT NOT NULL,
                phone TEXT NOT NULL,
                address TEXT NOT NULL,
                country TEXT NOT NULL
            );
        ''')

        conn.commit()
        print("User table created successfully")
    except:
        print("User table creation failed - Maybe table")
    finally:
        conn.close()