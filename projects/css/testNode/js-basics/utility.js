class BaseClass {
    Add(a, b) {
        return a + b;
    }

    GetData2() {
        return [ {name: 'Kim2', address: 'Korea2'}, {name: 'Park2', address: 'USA2'}]
    }
}

class Test extends BaseClass{
    Add(a, b) {
        return a + b;
    }

    GetData() {
        return [ {name: 'Kim', "address": 'Korea'}, {name: 'Park', address: 'USA'}]
    }
}

function testFunction(a, b) {
    return a + b;
}

let test = new Test();
let test2 = new BaseClass();

const _radius = new WeakMap();

class Circle {
    constructor(radius) {
        _radius.set(this, radius);
    }

    draw() {
        console.log('Circle with radius ' + _radius.get(this));
    }
}

module.exports = {Circle, test, test2, BaseClass, Test, testFunction};

