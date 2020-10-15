import { sayHello } from "./greet";
let options = {
	color: "red",
	volume: 11
}
let testString:string[] = [];

function showHello(divName: string, name: string) {
    const elt = document.getElementById(divName);
	elt.innerText += " " + sayHello(name);
}

showHello("greeting", "TypeScript");
showHello("greeting", options.color);
testString = ["abc", "def", "xyz"];
showHello("greeting", testString[0]);

let numbers = [1, 4, 9]; 
let roots = numbers.map(Math.sqrt); 
for (let i=0; i< roots.length; i++) {
	showHello("greeting", roots[i].toString());
}