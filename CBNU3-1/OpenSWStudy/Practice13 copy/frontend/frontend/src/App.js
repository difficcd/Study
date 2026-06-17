

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
    fetch("http://localhost:5000/products") 
      .then((res)=>res.json())
      .then((data)=>setProducts(data));
  }, []);

  function addProduct() {
    const newProduct = {
      title: title,
      price: price,
      image: image,
      category: category
    };

    fetch("http://localhost:5000/add-product", {
      method: "POST",
      headers: {
        "Content-Type": "application/json"
      },
      body: JSON.stringify(newProduct)
    })
      .then((res)=>res.json())
      .then((data)=>{
        console.log(data);
        alert("Product added successfully");
    });
  }

    function addReview() {
    const newReview = {
      Pid: Pid,
      Pname: Pname,
      Uname: Uname,
      review: review
    };

    fetch("http://localhost:5000/add-review", {
      method: "POST",
      headers: {
        "Content-Type": "application/json"
      },
      body: JSON.stringify(newReview)
    })
      .then((res)=>res.json())
      .then((data)=>{
        console.log(data);
        alert("Review added successfully");
    });
  }


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
      
        <input
          type="text" placeholder="Product title" value={title}
          onChange={(e)=> setTitle(e.target.value)}
        /><br/>

        <input
          type="number" placeholder="Product price" value={price}
          onChange={(e)=> setPrice(e.target.value)}
        /><br/>

        <input
          type="text" placeholder="Image URL" value={image}
          onChange={(e)=> setImage(e.target.value)}
        /><br/>

        <input
          type="text" placeholder="Category" value={category}
          onChange={(e)=> setCategory(e.target.value)}
        /><br/>

        <button onClick={addProduct}> Add Product </button><br/><br/>


        <div>
          <h1> ADD review Products </h1>

          <input
            type="text" placeholder="RV Product id" value={Pid}
            onChange={(e)=> setPid(e.target.value)}
          /><br/>
          <input
            type="text" placeholder="RV Product name" value={Pname}
            onChange={(e)=> setPname(e.target.value)}
          /><br/>
          <input
            type="text" placeholder="your name" value={Uname}
            onChange={(e)=> setUname(e.target.value)}
          /><br/>
          <input
            type="text" placeholder="review" value={review}
            onChange={(e)=> setReview(e.target.value)}
          /><br/>
          <button onClick={addReview}> Add Review </button><br/><br/>

        </div>


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
