const EventEmitter = require('events');

class MyParent extends EventEmitter {
    foo() {
        console.log("foo called");
        this.emit('MyEvent');
    }
}

class MyClass extends MyParent {
    bar() {
        var myArray = [
            { "name": "kim", "address": "korea" },
            { "name": "park", "address": "usa" }
        ]
        var kim = "test";
        kim += JSON.stringify(myArray);
        console.log(kim);
        this.emit("BarEvent");
    }
}

module.exports = MyClass;