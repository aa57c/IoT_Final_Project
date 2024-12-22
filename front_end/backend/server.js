const express = require('express');
const bodyParser = require('body-parser');
const cors = require('cors');

// Initialize Express app
const app = express();
const port = 5000;

// Middleware setup
app.use(cors())
app.use(bodyParser.json())

let sensorData = {
    worker_id: 'worker1',
    temp: '0',
    humidity: '0',
    gas_level: '0',
    collision_alert: "",
    emergency_alert: "",
    timestamp: ''
};

app.post('/api/send-sensor-data', (req,res) => {
    // Extract sensor data from request body
    const {temp, humidity, gas_level, collision_alert, emergency_alert} = req.body;
    sensorData = {
        worker_id: 'worker1',
        temp: temp,
        humidity: humidity,
        gas_level: gas_level,
        collision_alert: collision_alert,
        emergency_alert: emergency_alert,
        timestamp: new Date().toISOString()
    }
    res.status(200).json({message: "Sensor data recieved and updated"});
});

app.get('/api/recieve-sensor-data', (req, res) => {
    res.status(200).json(sensorData);
});

app.listen(port, () => {
    console.log(`Server is running on http://localhost:${port}`);
})
