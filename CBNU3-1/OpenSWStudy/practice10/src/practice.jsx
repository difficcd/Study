
function App() {
    const name = "Aziz";
    const score = 80;

    function getMessage(){
        return "Welcome to React JSX";
    }


    return (
        <>
         <h1> hello, {name} </h1>
         <p> {getMessage()} </p>
         <p> your next score = {score+10}</p>
        </>
    )
}

function App() {
    const temperatures = [12, 25, 18, 30, 5];
    const list = temperatures.map((item) =>
      <li>{item}℃ : {item >= 20 ? "Warm" : "Cold"}</li> );

    return (
      <>
        <h1> Daily Temperature Report </h1>
        <ul>
          {list}
        </ul>
      </>
    )
}


function App() {
    const clubs = [
      {
        image: "https://picsum.photos/200",
        name: "React for Beginners1",
        president: "Aziz Nasridinov",
        members: 45
      },
      {
        image: "https://picsum.photos/200",
        name: "React for Beginners2",
        president: "Aziz Nasridinov",
        members: 45
      },
      {
        image: "https://picsum.photos/200",
        name: "React for Beginners3",
        president: "Aziz Nasridinov",
        members: 45
      }
    ];

    const myList = clubs.map((club) =>
      <CourseCard image={club.image}
                    name={club.name}
                    president={club.president}
                    members={club.members}/> 
    );

    return (
      <>
        <Navbar />

        {myList}
       
        <Footer />
      </>
    )
}


function App() {
    const clubs = [
      {
        image: "https://picsum.photos/200",
        name: "BooK1",
        president: "info1",
        Liked: 12
      },
      {
        image: "https://picsum.photos/200",
        name: "BooK2",
        president: "info2",
        Liked: 49
      },
      {
        image: "https://picsum.photos/200",
        name: "BooK3",
        president: "info3",
        Liked: 38
      }
    ];

    const myList = clubs.map((club) =>
      <CourseCard   image={club.image}
                    name={club.name}
                    president={club.president}
                    Liked={club.Liked}
                    className ="mstyle"/> 
    );

    const showTopClub = () => {
      let topClub = clubs[0];
      const myList = clubs.map((club) => 
        { if(topClub.Liked < club.Liked) topClub = club }
      );
      alert("Top: " + topClub.name + " with "
             + topClub.Liked + " Liked");
    }

    return (
      <>
        <Navbar />
        <button className="show" onClick = {showTopClub}> show top Book</button>
        <br/><br/>

        <div className="layer"> {myList} </div>
        <Footer />
      </>
    )
}