let person = { name: 'Mosh' };

console.log(Object.keys(person));

let objectBase = Object.getPrototypeOf(person);
let descriptor = Object.getOwnPropertyDescriptor(objectBase, 'toString');
console.log(descriptor);

Object.defineProperty(person, 'name', {
    writable: false,
    enumerable: false,     
    configurable: false   // can't delete
});

person.name = 'John';
console.log(Object.keys(person));

function Shape(color = 'yellow') {
    this.color = color;
}

Shape.prototype.duplicate = function() {
    console.log('duplicate');
}

function Circle(radius, color) {
    Shape.call(this, color);   // Call constructor of parent
    // instance members
    this.radius = radius;
}
extend(Circle, Shape);

Circle.prototype.duplicate = function() {
    Shape.prototype.duplicate.call(this);
    console.log("My New duplicate for circle");
}

function extend(Child, Parent) {
    Child.prototype = Object.create(Parent.prototype);
    Child.prototype.constructor = Child;
}

// prototype members
Circle.prototype.draw = function() {
    console.log('draw');
}

const c = new Circle(1, 'red');
Circle.prototype === c.__proto__;
const s = new Shape();

function Square(size) {
    this.size = size;
}
extend(Square, Shape);

Square.prototype.duplicate = function() {
    Shape.prototype.duplicate.call(this);
    console.log("My New duplicate for square");
}

let sq = new Square();
sq.duplicate();

const shapes = [
    new Circle(),
    new Square()
];

for (let shape of shapes)
    shape.duplicate();

const canEat = {
    eat: function() {
        this.hunger--;
        console.log('eating');
    }
}
const canWalk = {
    walk: function() {
        console.log('walking');
    }
}
const canSwim = {
    swim: function() {
        console.log('swiming');
    }
}

function mixin(target, ...sources) {
    Object.assign(target, ...sources);
}

const p = mixin({}, canEat, canWalk);
console.log(p);

function Animal() {
}
mixin(Animal.prototype, canEat, canWalk);
const p2 = new Animal();
p2.eat();

function Goldfish() {
}
mixin(Goldfish.prototype, canEat, canSwim);
const p3 = new Goldfish();
p3.eat();
p3.swim();


function HtmlElement() {
    this.click = function() {
        console.log('clicked');
    }
}

HtmlElement.prototype.focus = function() {
    console.log('focused');
}

function HtmlSelectElement(items = []) {
    this.items = items;

    this.addItem = function(item) {
        this.items.push(item);
    }

    this.removeItem = function(item) {
        this.items.splice(this.items.indexOf(item), 1);
    }

    this.render = function() {
        return `
<select>${this.items.map(item => `
  <option>${item}</option>`).join('')}
</select>`;
    }
}

HtmlSelectElement.prototype = new HtmlElement();
//HtmlSelectElement.prototype = Object.create(HtmlElement.prototype);

function HtmlImageElement(src) {
    this.src = src;
    this.render = function() {
        return `<image src="${this.src}" />`
    }
}

HtmlImageElement.prototype = new HtmlElement();
HtmlImageElement.prototype.constructor = HtmlImageElement;

