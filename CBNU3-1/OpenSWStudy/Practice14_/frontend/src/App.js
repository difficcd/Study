

import {useState,useEffect} from "react";

function App() {
  const [message, setMessage] = useState("");
  const [products, setProducts] = useState([]);

  const [title, setTitle] = useState("");
  const [price, setPrice] = useState("");
  const [image, setImage] = useState("");
  const [category, setCategory] = useState("");

  const [Pid, setPid] = useState("");
  const [Pname, setPname] = useState("");
  const [Uname, setUname] = useState("");
  const [review, setReview] = useState("");
  
  useEffect(() => {
    fetch("http://localhost:5000/jewelery") 
      .then((res)=>res.json())
      .then((data)=>setProducts(data));
  }, []);


    const style = {
      width: '100px',
      height: '100px',
      padding: '10px',
      margin: '10px',
      border: '2px solid black',
      borderRadius: '10px',
    };

  return (
    <div className="App">
      <h1>Product List</h1>

      <div className="product-container">
    
     

        {products.map((product) => {
          return(
            <div className="product-card" key={product.id}>
                <img style={style} src={product.image} alt="product" />
                <h2>{product.title}</h2>
                <p>Prcie : ${product.price}</p>
            </div>
          )
        })}
      </div>
    </div>
  );
}

export default App;
