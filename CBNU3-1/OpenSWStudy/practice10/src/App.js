import logo from './logo.svg';
import './App.css';
import React from 'react';
import ReactDOM from 'react-dom/client';

import Navbar from './Navbar.jsx';
import CourseCard from './ClubCard.jsx';
import Footer from './Footer.jsx';

import {useState, useEffect} from "react";



function App() {
    const clubs = [
      {
        image: "https://picsum.photos/200",
        name: "Laptop",
        price: 900
      },
      {
        image: "https://picsum.photos/201",
        name: "Keyboard",
        price: 50
      },
      {
        image: "https://picsum.photos/202",
        name: "Mouse",
        price: 30
      }
    ];

    const [selected, setSelect] = useState(0); 
    const [price, setPrice] = useState(0); 
    const [search, setSearch] = useState(""); 

    // Task3
    const [user, setUser] = useState(null);
    const [loading, setLoading] = useState(true);

    useEffect(() => {
      fetch("https://fakestoreapi.com/products")
      .then((res) => res.json())
      .then((data) => {
        setUser(data); // save user data
        setLoading(false); // loading finished
      })
    }, []);  //~03
  
    const myList = user?.map((user) =>
      <CourseCard   image={user.image}
                    name={user.title}
                    price={user.price}
                    var1={selected} fun1={setSelect}
                    var2={price} fun2={setPrice}
                    search={search}
                    /> 
    );

    const myList_ = clubs.map((club) =>
      <CourseCard   image={club.image}
                    name={club.name}
                    price={club.price}
                    var1={selected} fun1={setSelect}
                    var2={price} fun2={setPrice}
                    search={search}
                    className ="mstyle"/> 
    );


     const mystyle = {
        padding: "10px",
        margin: '20px',
        borderRadius: "10px",
    }

    const container = {
      margin: "10px",
      textAlign: "center",
    }

    if (loading) return <p>Loading...</p>  

    return (
      <>
        <Navbar/>
          <form>
            <label>
              <input type="text" style={mystyle} 
                      value = {search}
                      onChange={ (e)=> setSearch(e.target.value)} />
            </label>
          </form><br /><br />

         <div style={container}> {myList} </div>
          <h3>&nbsp;&nbsp; Selected Products: {selected}</h3>
          <h3>&nbsp;&nbsp; Total Price: ${price}</h3>
        <Footer/>
      </>
    )
}

const root = ReactDOM.createRoot(document.getElementById('root'));
root.render(<App />)
export default App;