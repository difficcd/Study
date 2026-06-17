

const express = require("express");
const app = express();


// ## 1 Express 식 서버 열기!
app.get("/", function(req, res) { res.send("Home Page"); }); // 웹에서 뜰 것
app.listen(3000, function() {
    console.log("Server is running on port 3000 WITH Express!\n"); // 콘솔에서 뜰 것
});


// ## 2 기본 Route 추가
/*app.get("/products", function(req, res) {
    res.send("Product List");
});*/ // 아래에 있는 애 때문에 같이쓰면 얘로돼서 주석처리
app.get("/users", function(req, res) {
    res.send("User List");
});


// ## 3 Route 확장 GET,POST,PUT,DELETE
app.get("/users/:id", function(req,res) {
    const id = req.params.id;
    res.send("User ID is " + id );
    
    // ###  경로에서 정보 추출하기   -   경로/:Params
    // req.params 는 자동으로 파싱해서 줌
    // :ADD => params.ADD 꼴로 꺼내기 가능!!
})
app.get("/products", function(req, res) {
    const category = req.query.category;
    res.send("Category is " + category);
    // ###  경로에서 정보 추출하기   -   경로/products?category=phone
}) 
app.put("/users/:id", function(req, res) {
    const id = req.params.id;
    res.send("Update user " + id);
});
app.delete("/users/:id", function(req, res) {
    const id = req.params.id;
    res.send("Delete user " + id);
}); 


// ## 4 로깅 미들웨어 + 로그인 본문 받기
app.use((req, res, next) => {
    console.log(`[로그] ${req.method} ${req.url}\n`);
    next(); 
}); // next: 미들웨어에서 멈추기방지 (넘겨주는애)

app.use(function(req, res, next) {
    console.log("A request came to the server\n");
    next();
});

app.use(express.json()); 
/* {
  "email": "aziz@cbnu.ac.kr",
  "password": "12345"
    } 
  =>  req.body(json)에서 email, password 추출(파싱)
*/

app.post("/login", function(req, res) {
    const email = req.body.email;
    const password = req.body.password;
    console.log(email);
    console.log(password);

    res.send("Login data received from " + email);
});



// ## 5 DB 연결 => DB & Table 생성
var mysql = require("mysql");

var con = mysql.createConnection({
    host: "localhost",
    user: "root",
    password: "1234",
    database: "mydb",
}); // => DB연결을 위해 필요한 것들 : con



con.connect(function (err) { // DB 연결: con(정보).connect
    if (err) throw err;
    console.log("DB Connected\n");
})

app.get("/create-database", function(req, res) {
    con.query(`CREATE DATABASE IF NOT EXISTS mydb`, function(err, result){
        if (err) throw err;
            res.send("DB checked/created");
    })
})

app.get("/create-table", function (req, res) {
    console.log("DB Connected!\n");
    var sql = `CREATE TABLE IF NOT EXISTS customers (
            id INT AUTO_INCREMENT PRIMARY KEY,
            name VARCHAR(255),
            address VARCHAR(255)
        )`  // varchar(N) : 가변길이, 최대 N글자까지 저장가능

    con.query(sql, function (err, result) { // 쿼리 전송: con(정보).query
      if (err) throw err;
            console.log("\nResult: " + result +"\n");

            res.send( "Table checked/created" );
    });
});