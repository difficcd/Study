
var express = require("express");
var cors = require("cors");
var mysql = require("mysql2");

var app = express();


app.use(cors());
app.use(express.json());

app.get("/", function(req, res) { 
    res.send("Hello Express"); 
});

var con = mysql.createConnection({
    host: "localhost",
    port: 3306,
    user: "root",
    password: "1234",
    database: "task_db"
})

con.connect(function (err) { 
    if (err) throw err;
    console.log("DB Connected!\n");
})

/*
app.get("/products", function(req, res) {
    var sql = "SELECT * FROM products";
    con.query(sql, function(err, result){
        if (err) throw err;
        res.send(result);
    });
});

app.get("/:category", function(req,res) {
    const category = req.params.category;
    var sql = "SELECT * FROM products " +
               "WHERE category = '" + category + "'";
    con.query(sql, function(err, result){
        if(err) throw err;
        res.send(result);
    })
})

app.post("/add-product", function (req,res){
    const title = req.body.title;
    const price = req.body.price;
    const image = req.body.image;
    const category = req.body.category;

    const sql = `INSERT INTO products (title, price, image, category) VALUES (?, ?, ?, ?)`;
    con.query(sql, [title, price, image, category], function(err,result) {
        if(err){
            return res.json(err);
        }
        return res.json({
            message: "Product inserted successfully"
        });
    });
});*/

app.post("/add-review", function (req,res){
    const Pid = req.body.Pid;
    const Pname = req.body.Pname;
    const Uname = req.body.Uname;
    const review = req.body.review;

    
    const sql = `INSERT INTO reviews (Pid, Pname, Uname, review) VALUES (?, ?, ?, ?)`;
    con.query(sql, [Pid, Pname, Uname, review], function(err,result) {
        if(err){
            return res.json(err);
        }
        return res.json({
            message: "Review inserted successfully"
        });
    });
});


app.listen(5000, function() {
    console.log("Server is running on port 5000");
})
