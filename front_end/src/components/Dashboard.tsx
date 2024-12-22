import React, { useEffect, useState } from "react";
import axios from "axios";
import { Pie } from "react-chartjs-2";
import { Chart as ChartJS, Title, Tooltip, Legend, ArcElement } from "chart.js";
import './dashboard.css';

// Register Chart.js components
ChartJS.register(Title, Tooltip, Legend, ArcElement);

interface SensorData {
    worker_id: string;
    temp: string;
    humidity: string;
    gas_level: string;
    collision_alert: string;
    emergency_alert: string;
    timestamp: string;
}

const Dashboard: React.FC = () => {
    const [data, setData] = useState<SensorData | null>(null);
    const [chartData, setChartData] = useState<any>([]);

    // Update chart data for the latest sensor reading
    const updateChartData = (sensorData: SensorData) => {
        const temp = parseFloat(sensorData.temp);
        const humidity = parseFloat(sensorData.humidity);
        const gasLevel = parseFloat(sensorData.gas_level);
        const maxGasLevel = 100;
        const maxTemp = 50; // max temperature value
        const maxHumidity = 100; // max humidity value

        // Set chart data for all three (gas, temp, humidity)
        setChartData([
            {
                labels: ["Gas Level"],
                datasets: [
                    {
                        data: [gasLevel, maxGasLevel - gasLevel],
                        backgroundColor: ["#FF5733", "#ddd"],
                        hoverOffset: 4,
                    },
                ],
                options: {
                    rotation: Math.PI,  
                    circumference: Math.PI, 
                },
            },
            {
                labels: ["Temperature"],
                datasets: [
                    {
                        data: [temp, maxTemp - temp],
                        backgroundColor: ["#FFCC00", "#ddd"],
                        hoverOffset: 4,
                    },
                ],
                options: {
                    rotation: Math.PI,  
                    circumference: Math.PI, 
                },
            },
            {
                labels: ["Humidity"],
                datasets: [
                    {
                        data: [humidity, maxHumidity - humidity],
                        backgroundColor: ["#00BFFF", "#ddd"],
                        hoverOffset: 4,
                    },
                ],
                options: {
                    rotation: Math.PI,  // Start from the top (half pie)
                    circumference: Math.PI, // Half the pie
                },
            },
        ]);
    };

    // Function to categorize the readings into safe/unsafe
    const categorizeReading = (value: number, type: string) => {
        switch (type) {
            case "gas":
                if (value <= 20) return { status: "safe", color: "green" };
                if (value <= 50) return { status: "moderate", color: "yellow" };
                return { status: "unsafe", color: "red" };
            case "temp":
                if (value >= 15 && value <= 30) return { status: "safe", color: "green" };
                if (value > 30) return { status: "unsafe", color: "red" };
                return { status: "moderate", color: "yellow" };
            case "humidity":
                if (value >= 30 && value <= 60) return { status: "safe", color: "green" };
                return { status: "unsafe", color: "red" };
            default:
                return { status: "safe", color: "green" };
        }
    };

    // Fetching sensor data
    useEffect(() => {
        const fetchData = async () => {
            try {
                const response = await axios.get<SensorData>("http://localhost:5000/api/recieve-sensor-data");
                setData(response.data);
                updateChartData(response.data);
            } catch (error) {
                console.error("Error fetching sensor data:", error);
            }
        };

        // Fetch data every 2 seconds
        const intervalId = setInterval(fetchData, 2000);

        // Cleanup the interval on component unmount
        return () => clearInterval(intervalId);
    }, []);

    function capitalizeFirstLetter(str: string | undefined){
        if (str == null){
            return;
        }
        return str.charAt(0).toUpperCase() + str.slice(1).toLowerCase();

    }

    return (
        <div className="dashboard">
            <div className="heading"> Mining Safety Dashboard For {capitalizeFirstLetter(data?.worker_id)} </div>
            {data && (
                <>
                    {/* Alert Section */}
                    <div className="alerts">
                        {(data.collision_alert === "true") && (
                            <div className="alert alert-collision">
                                <h4>Collision Alert!</h4>
                                <p>Impact detected. Please check the area.</p>
                            </div>
                        )}

                        {(data.emergency_alert === "true") && (
                            <div className="alert alert-emergency">
                                <h4>Emergency Alert!</h4>
                                <p> I need help. Please assist immediately.</p>
                            </div>
                        )}
                    </div>

                    {/* Pie Charts and Sensor Status */}
                    <div className="readings-emergencyalert">
                        <div className="pie-chart-container">
                            <h3>Sensor Readings</h3>
                            <div className="pie-sensor-readings">
                                <div style={{ width: "170px", height: "170px" }}>
                                    <Pie data={chartData[0]} options={{ rotation: 270, circumference: 180 }} />
                                </div>
                                <div style={{ width: "170px", height: "170px" }}>
                                    <Pie data={chartData[1]} options={{ rotation: 270, circumference: 180 }} />
                                </div>
                                <div style={{ width: "170px", height: "170px" }}>
                                    <Pie data={chartData[2]} options={{ rotation: 270, circumference: 180 }} />
                                </div>
                            </div>
                        </div>

                        {/* Sensor Status */}
                        <div className="sensor-status">
                            <h3>Sensor Status</h3>
                            <div
                                className="sensor-reading"
                                style={{
                                    backgroundColor: categorizeReading(parseFloat(data.gas_level), "gas").color,
                                }}
                            >
                                <h4>Gas Level: {parseFloat(data.gas_level).toFixed(2)} ppm</h4>
                                <p>Status: {categorizeReading(parseFloat(data.gas_level), "gas").status}</p>
                            </div>
                            <div
                                className="sensor-reading"
                                style={{
                                    backgroundColor: categorizeReading(parseFloat(data.temp), "temp").color,
                                }}
                            >
                                <h4>Temperature: {parseFloat(data.temp).toFixed(2)} °C</h4>
                                <p>Status: {categorizeReading(parseFloat(data.temp), "temp").status}</p>
                            </div>
                            <div
                                className="sensor-reading"
                                style={{
                                    backgroundColor: categorizeReading(parseFloat(data.humidity), "humidity").color,
                                }}
                            >
                                <h4>Humidity: {parseFloat(data.humidity).toFixed(2)} %</h4>
                                <p>Status: {categorizeReading(parseFloat(data.humidity), "humidity").status}</p>
                            </div>
                        </div>
                    </div>
                </>
            )}
        </div>
    );
};

export default Dashboard;
