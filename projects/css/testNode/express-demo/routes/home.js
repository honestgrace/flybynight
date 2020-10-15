const express = require('express');
const router = express.Router();

router.get('/', (req, res) => {
    //res.send('Hello World');
    res.render('index', {title: 'My Express App', message: 'Hello'});  // index.pug
});

router.get('/api/posts/:year/:month', (req, res) => {
    //res.send(req.params);
    //http://localhost/api/posts/2018/1?sortBy=name
    res.send(req.query);
});

module.exports = router;