


// #1 fs module (built-in)
const fs = require("fs");
fs.writeFile("message.txt", "Hello Node.js", function(err) {
    if (err) throw err;
    console.log("File saved");
});

// #2 path module => /,\ 구분 모호함 해결 
const path = require("path");
const filePath = path.join(
    __dirname, // 이게 중요함
    "files",
    "data.txt"
);
console.log(filePath);

// #3 http module => ### server 만들기
const http = require("http");
const server = http.createServer(function(req, res) {
    res.end("Hello from Node.js server");
});
server.listen(3000);



// Part 03 node.js

// without express (raw http)
const http = require("http");
const server = http.createServer(
    function(req, res) {
    if (req.url === "/" &&
        req.method === "GET") {
            res.end("Hello from Node.js");
        }
    }
); server.listen(3000);

// with express!!   ### Express 사용법
app.get("/", function(req, res) {
    res.send("Home Page");
});

// express hello world  ### Express 1
const express = require("express");
const app = express();
app.get("/", function(req, res) {
    res.send("Hello Express");
});
app.listen(3000, function() {
    console.log("Server is running on port 3000");
});



// Express Routing 
app.get("/", function(req, res) {
    res.send("Home Page");
});
app.get("/products", function(req, res) {
    res.send("Product List");
});
app.get("/users", function(req, res) {
    res.send("User List");
});

// get(사용자가 데이터 retreive 하고싶을때)
// post(사용자의 새 데이터를 서버에 적고싶을때)
// put(update data), delete(그대로 삭제) 
app.post("/login", function(req, res) {
    res.send("Login data received");
});
app.put("/users/:id", function(req, res) {
    res.send("Update user");
});
app.delete("/users/:id", function(req, res) {
    res.send("Delete user");
}); 

  // id 는 ? => 조건 [33p]


  // params (url parameters 34p)
app.get("/users/:id", 
    function(req, res) {
        const id = req.params.id;
        res.send("User ID is " + id);
    }
); // 동적 route (all case id 대응)


 // Query Strings ( 36p )
app.get("/products", function(req, res) {
    const category = req.query.category;
    res.send("Category is " + category);
}) 
 // http://localhost:3000/products?category=phone
 // then req.query.category will equal "phone".


 // Middleware => 필요성: lock 
 // 1) 중간 report = (req왔을때 res전에 req받음, 처리상태등 알림)
 // 2) 제일많이사용하는경우 = 아래참조
app.use(function(req, res, next) {
    console.log("A request came to the server");
    next();
});

app.get("/", function(req, res) {
    res.send("Home Page");
});

app.get("/users", function(req, res) {
    res.send("User List");
});


    // 2) 이 json 코드의 문제는? 
    //    => react = all json으로 data보냄 
    //    => DB랑 format 매칭X(json파싱필요)
    //    => 이 번역(파싱)을 미들웨어가 해주는 것
    // * 중요: frontend=request (back에는없음)
    fetch("http://localhost:3000/login", {
        method: "POST",
        headers: {
        "Content-Type": "application/json"
        },
        body: JSON.stringify({
            email: "aziz@cbnu.ac.kr",
            password: "12345"
        })
    })

    const express = require("express");
    const app = express();
    app.use(express.json()); 
     // parse JSON bodies (middleware)
     // parsing => req.body 

    app.post("/login", function(req, res) {
        const email = req.body.email;
        const password = req.body.password;

        console.log(email);
        console.log(password);

        res.send("Login data received");
    });
    app.listen(3000);



// part 04     
    // DB structure (MySQL : 45-46p)
    // Creating mySQL Connection
var express = require("express");
var mysql = require("mysql"); // import mysql 
var app = express();

var con = mysql.createConnection({
    host: "localhost",
    user: "yourusername",
    password: "yourpassword"
}); // => DB연결을 위해 필요한 것들 : con

app.get("/connect", function(req, res) {
    con.connect(function(err) {
        if (err) throw err;
        console.log("Connected!");
        res.send("Connected!");
    }); // 위의 con 으로 con.connect
}); // connection 

// Querying the DB
app.get("/run-query", function(req, res) {
    con.connect(function(err) {
        if (err) throw err;
        console.log("Connected!");
        con.query(sql, function(err, result) {
            if (err) throw err;
            console.log("Result: " + result);
            res.send(result);
        }); // query basic. sql 부분에 쿼리 작성 ("query")
    });
});

// create DB in mySQL ... (use query)
app.get("/create-database", function(req, res) {
    con.connect(function(err) {
        if (err) throw err;
        console.log("Connected!");
        con.query("CREATE DATABASE mydb",
            function(err, result) {
            if (err) throw err;
            console.log("Database created");
            res.send("Database created");
            }
        );
    });
});

// connecting to a specific DB
var express = require("express");
var mysql   = require("mysql");
var app     = express();
var con = mysql.createConnection({
host:     "localhost",
user:     "yourusername",
password: "yourpassword",
database: "mydb"       // ← specify the database here
});



// FULL EXAMPLE
app.get("/create-table", function(req, res) {
    con.connect(function(err) {
        if (err) throw err;
        var sql = "CREATE TABLE customers (" +
                    "id INT PRIMARY KEY, " +
                    "name VARCHAR(255), " +
                    "address VARCHAR(255)" +
                    ")";
        con.query(sql, function(err, result) {
            if (err) throw err;
            console.log("Table created");
            res.send("Table created");
        });
    });
});


// #1 INSERT
app.get("/insert-customer", function(req, res) {
    con.connect(function(err) {
    if (err) throw err;
    var sql = "INSERT INTO customers " +
                "(id, name, address) " +
                "VALUES (1, 'Company Inc', 'Highway 37')";
    con.query(sql, function(err, result) {
        if (err) throw err;
        console.log("1 record inserted");
        res.send("1 record inserted");
        });
    });
})

// #2 SELECT
app.get("/customers", function(req, res) {
    con.connect(function(err) {
        if (err) throw err;
        var sql = "SELECT name, address FROM customers";
        con.query(sql, function(err, result, fields) {
            if (err) throw err;
            console.log(result);
            res.send(result); 
            // sends JSON array to frontend
        });
    });
});

// #3 Field Object
app.get("/customer-field", function(req, res) {
    con.connect(function(err) {
        if (err) throw err;
        var sql = "SELECT name, address FROM customers";
        con.query(sql, function(err, result, fields) {
            if (err) throw err;
            console.log(fields[0].name);  // logs: "name"
            res.send(fields[0].name);
        });
    });
})

// #4 WHERE filtering records
app.get("/customers-by-address", function(req, res) {
    con.connect(function(err) {
        if (err) throw err;
        var sql = "SELECT * FROM customers " +
                  "WHERE address = 'Park Lane 38'";
        con.query(sql, function(err, result, fields) {
            if (err) throw err;
            console.log(result);
            res.send(result);
        });
    });
});

// #5 delete
app.get("/delete-customer", function(req, res) {
    con.connect(function(err) {
        if (err) throw err;
        var sql = "DELETE FROM customers " +
                  "WHERE address = 'Mountain 21'";
        con.query(sql, function(err, result) {
            if (err) throw err;
            console.log("Deleted: " + result.affectedRows);
            res.send("Records deleted: " + result.affectedRows);
        });
    });
})

// #6 update
app.get("/update-customer", function(req, res) {
    con.connect(function(err) {
        if (err) throw err;
        var sql = "UPDATE customers " +
                    "SET address = 'Canyon 123' " +
                    "WHERE address = 'Valley 345'";
        con.query(sql, function(err, result) {
            if (err) throw err;
            console.log(result.affectedRows + " record(s) updated");
            res.send(result.affectedRows + " record(s) updated");
        });
    });
})
