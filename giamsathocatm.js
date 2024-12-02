

//thiet bi 1

let btn1 = document.querySelector('#btn1');
let img1 = document.querySelector('#den');
let btn2 = document.querySelector('#btn2');
btn1.addEventListener('click', ()=>{
    img1.src = 'densang.png'; 
    firebase.database().ref("thietbi1").set({ den: 1 });
})
btn2.addEventListener('click', ()=>{
    img1.src = 'dentoi.png';
    firebase.database().ref("thietbi1").set({ den: 0 });
})

//thiet bi 2
let btn3 = document.querySelector('#btn3');
let img2 = document.querySelector('#oxi');
let btn4 = document.querySelector('#btn4');
btn3.addEventListener('click', ()=>{
    img2.src = 'openoxi.png'; 
    firebase.database().ref("thietbi2").set({ oxi: 1 });
})
btn4.addEventListener('click', ()=>{
    img2.src = 'closeoxi.png';
    firebase.database().ref("thietbi2").set({ oxi: 0 });
})
//thiet bi 3
let btn5 = document.querySelector('#btn5');
let img3 = document.querySelector('#bom');
let btn6 = document.querySelector('#btn6');
btn5.addEventListener('click', ()=>{
    img3.src = 'openbom.png'; 
    firebase.database().ref("thietbi3").set({ bom: 1 });
})
btn6.addEventListener('click', ()=>{
    img3.src = 'closebom.png';
    firebase.database().ref("thietbi3").set({ bom: 0 });
})