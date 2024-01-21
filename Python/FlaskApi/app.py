#!/usr/bin/python
import sqlite3
from flask import Flask, request, jsonify
from flask_cors import CORS


def connect_to_db():
    conn = sqlite3.connect('database.db')
    return conn
def insert_user(user):
    try:
        conn = connect_to_db()
        cur = conn.cursor()
        cur.execute("INSERT INTO MonitorCuaca (Temperature, Humidity, LightValue, RoomId) VALUES (?, ?, ?, ?)", (user['name'], user['email'], user['phone'], user['address'], user['country']) )
        conn.commit()
    except:
        conn().rollback()

    finally:
        conn.close()

    return 'Done'

app = Flask(__name__)
CORS(app, resources={r"/*": {"origins": "*"}})

@app.route('/api/users/',  methods = ['POST'])
def api_add_user():
    user = request.get_json()
    return jsonify(insert_user(user))

if __name__ == "__main__":
    #app.debug = True
    #app.run(debug=True)
    app.run()