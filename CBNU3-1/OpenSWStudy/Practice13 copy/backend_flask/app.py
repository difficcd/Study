

from flask import Flask
from flask import request
from flask import jsonify ## middleware
import pymysql


app = Flask(__name__)

@app.route("/")
def home():
    return "Hello Flask"

@app.route("/products")
def get_products():
    con = get_connection()
    cur = con.cursor()
    cur.execute("SELECT * FROM products")
    result = cur.fetchall()
    cur.close(); con.close()
    return jsonify(result)

@app.route("/login", methods=["POST"])
def login():
    data = request.json
    email = data["email"]
    password = data["password"]
    return "Login received"

@app.route("/test") # methods = GET
def test():
    return jsonify([
    { "id": 1, "name": "Laptop" },
    { "id": 2, "name": "Mouse" }
    ])


## DB
def get_connection():
    return pymysql.connect(
    host="localhost",
    user="root",
    password="1234",
    database="store_db",
    cursorclass=pymysql.cursors.DictCursor
    )

@app.route("/products/<id>", methods=["PUT"])
def update_product(id):
    data = request.json
    sql = "UPDATE products SET price = %s WHERE id = %s"
    con = get_connection(); cur = con.cursor()
    cur.execute(sql, (data["price"], id))
    con.commit(); cur.close(); con.close()
    return "Product updated"


@app.route("/products", methods=["POST"])
def add_product():
    data = request.json
    sql = "INSERT INTO products (title, price) VALUES (%s, %s)"
    con = get_connection(); cur = con.cursor()
    cur.execute(sql, (data["title"],data["price"]))
    con.commit(); cur.close(); con.close()
    return "Product added"


@app.route("/products/<id>", methods=["DELETE"])
def delete_product(id):
    sql = "DELETE FROM products WHERE id = %s"
    con = get_connection(); cur = con.cursor()
    cur.execute(sql, (id,))
    con.commit(); cur.close(); con.close()
    return "Product deleted"


app.run(port=5000)




# @app.route("/users")

# @app.route("/login",
#  methods=["POST"])

# @app.route("/users/<id>",
#  methods=["PUT"])

# @app.route("/users/<id>",
#  methods=["DELETE"]) 
