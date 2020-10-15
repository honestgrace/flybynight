const express=require('express');
const router= express.Router();


const courses = [
    { id: 1, name: 'course1'},
    { id: 2, name: 'course2'},
    { id: 3, name: 'course3'},
]

router.get('/', (req, res) => {
    res.send(courses);
});

router.get('/:id', (req, res) => {
    const course = courses.find(c=> c.id === parseInt(req.params.id));
    if (!course) res.status(404).send('The course with the given ID was not found!!!');
    else res.send(course);
});



function validateCourse(course) {
    const schema = {
        name: Joi.string().min(3).required()
    }
    return Joi.validate(course, schema);
}

router.post('/', (req, res)=> {
    const { error } = validateCourse(req.body); // result.error
    if (error) {
    //if (!req.body.name || req.body.name.length<3) {
    //if (result.error) {
        // 400 bad request
        //res.status(400).send(result.error);
        return res.status(400).send(error.details[0].message);
    }

    const course = {
        id: courses.length + 1,
        name: req.body.name
    };
    courses.push(course);
    res.send(course);
})

router.put('/:id', (req, res) => {
    let course = courses.find(c => c.id === parseInt(req.params.id));
    if (!course) return res.status(404).send('The course with the given ID was not found!!!');

    // object destructuring
    //const result = validateCourse(req.body);
    const { error } = validateCourse(req.body); // result.error

    //if (result.error) {
    if (error) {
        // 400 bad request
        //res.status(400).send(result.error);
        return res.status(400).send(error.details[0].message);
    }

    // Update course
    course.name = req.body.name;
    res.send(course);
})

router.delete('/:id', (req, res) => {
    let course = courses.find(c => c.id === parseInt(req.params.id));
    if (!course) return res.status(404).send('The course with the given ID was not found!!!');
    
    const index = courses.indexOf(course);
    courses.splice(index, 1); // remove one
    res.send(course);
})

module.exports = router;