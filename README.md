# IoT Mark1 Restful API

This is a Restful API for getting things' values and process values into Influxdb.

Therefore you can store temperature, humidity, CO, GAS to this TSDB for a long time range and visualize it by Grafana InfluxDB datasource.



## Support sensors

- MQ series: MQ2, MQ7, MQ135

- DHT series: DHT11, DHT22 , AM2302



## Requirements

- python 2.7 , flask module , flask_restful module
- Devices with GPIO
- Sensors



## API 

In order to bring new features to users over time, the IoT-Mark1 API supports versioning. 

| method | url         | Description                |
| ------ | ----------- | -------------------------- |
| GET    | /v1/iot-mk1 | List current data          |
| POST   | /v1/iot-mk1 | Write current data into DB |



------

## API Request and Respond

##### Request example:

```
curl -i -X POST -H "Content-Type: application/json" -d @iot-mk1-data.json http://127.0.0.1:7878/v1/iot-mk1
```



iot-mk1-data.json:*

```
{
    "location": "Gobin-4F12", 
    "measurement": "temperature", 
    "place": "My Desktop", 
    "value": 40
}
```



##### Responds Header and Body:

```
HTTP/1.0 201 CREATED
Content-Type: application/json
Content-Length: 180
Server: Werkzeug/0.12.2 Python/2.7.12
Date: Mon, 03 Jul 2017 09:43:34 GMT

[
    "Write into DB successfully : ", 
    {
        "location": "Gobin-4F12", 
        "measurement": "temperature", 
        "place": "My Desktop", 
        "value": 20
    }
]
```



## License

Apache License 2.0



## Author Information

Tony Cheng     Email: <tony.pig@gmail.com>
