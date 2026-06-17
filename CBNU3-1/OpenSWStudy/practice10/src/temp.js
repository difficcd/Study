import logo from './logo.svg';
import './App.css';
import React from 'react';
import ReactDOM from 'react-dom/client';

import Navbar from './Navbar.jsx';
import CourseCard from './ClubCard.jsx';
import Footer from './Footer.jsx';

import {useState, useEffect, useRef} from "react";


function App() {
    const [color, setColor] = useState("red");

    // Practice2 
    const [name, setName] = useState(""); //input1
    const [email, setEmail] = useState(""); //input2
    function showInfo(e){
      alert("name: " + name + "\nEmail: " + email);
      e.preventDefault();
    } // ~2


    // useEffect
    const [count, setCount] = useState(0);
    // useEffect = render 한 이후에 추가적렌더링 (side effect)
    useEffect (() => {
      document.title = `you clicked ${count} times`;
    }, [count]); // dependency array => {count}


    // 32p useEffect(2) => Practice03
    const [user, setUser] = useState(null);
    const [loading, setLoading] = useState(true);

    useEffect(() => {
      fetch("https://jsonplaceholder.typicode.com/users/1")
      .then((res) => res.json())
      .then((data) => {
        setUser(data); // save user data
        setLoading(false); // loading finished
      })
    }, []);  //~03

    // useRef
    const inputRef = useRef(null);

    const handleClear = () => {
      inputRef.current.value = "";
      inputRef.current.focus();
    }

    if (loading) return <p>Loading...</p>  // Prac03!!

    return (
      <>
        {/* 01 */}
        <h1>TEST is {color}</h1>   
        <button
          type = "button"
          onClick={()=>setColor("blue")}
          >Blue</button> 
         <br /><br /><br /><br />
        
        {/* Practce02 */}
        <form>
          <label>
            <input type="text"
                    value = {name}
                    onChange={ (e)=> setName(e.target.value)} />
            <br /><br />
            <input type="text"
                    placeholder = "Enter email"
                    onChange={ (e)=> setEmail(e.target.value)} />
            <br /><br />
            <button onClick={showInfo}>Show</button>
          </label>
        </form> <br /><br />

        {/* 03 */}
        <p> you clicked {count} times</p>
        <button onClick={() => setCount(count + 1)}>
          Click me
        </button>

        {/* 04  => Practice3 */}
        <div>
          <h2>{user.name}</h2>
          <p>Email: {user.email}</p>
          <p>City: {user.address.city}</p>
        </div>

        {/* 04 */}
        <div>
          <input ref={inputRef} placeholder="Search..."/>
          <button onClick={handleClear}>Clear</button>
        </div>
      </>
    )
}

const root = ReactDOM.createRoot(document.getElementById('root'));
root.render(<App />)
export default App;