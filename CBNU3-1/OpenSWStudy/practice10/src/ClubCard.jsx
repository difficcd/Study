

import {useState, useEffect, useRef} from "react";


function CourseCard(props){

    

    const mystyle = {
        border: "2px solid black",
        backgroundColor: "lightgray",
        padding: "20px",
        margin: '10px',
        borderRadius: "20px",
        display: "inline-block",
        width: "200px", 
        height: "300px",
        fontSize : "10px",
        overflow : "hidden",
    }

    const imgstyle = {

        height: "100px"
    }
    
    function calculate() {
        props.fun1(props.var1+1);
        props.fun2(props.var2+props.price);
    }

    if( props.name.includes(props.search) ){
       return (
            <>
                <div style={mystyle} className="course-card">
                    <img style={imgstyle} src={props.image} />
                    <h2>{props.name}</h2>
                    <p>Price: ${props.price}</p>

                    <button onClick={calculate}>
                    select Product
                    </button>
                </div>
            </>
        );  
    }
    else return (<></>);
}

export default CourseCard;