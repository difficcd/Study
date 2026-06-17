

const fs = require("fs");
const path = require("path");
const http = require("http");

// #1 log.txt '로깅 파일'에 서버작동 기록
fs.writeFile("log.txt", "Server started", function(err) {
    if (err) throw err;
    console.log("File saved");
});

const filePath = path.join( __dirname, "log.txt");
console.log(filePath); 
// __dirname = 현재 파일이 있는 폴더 절대경로 (Node.js변수)
// 위에서 쓴 log.txt 경로가 위와 같음.


// #2 포트 300으로 서버 띄우기 (http 방식)
const server = http.createServer(function(req, res) {
    res.end("Hello from Node.js server");
});
server.listen(3000);

