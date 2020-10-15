const MyClass = require ('./utility_module');
const myClass = new MyClass();
myClass.on('MyEvent', function() {
    console.log('My Event is called');
});

myClass.on('BarEvent', () => {
    console.log('Bar Event is also called');
})

myClass.foo();
myClass.bar();