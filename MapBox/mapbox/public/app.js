import React, { useEffect } from 'react';
import mapboxgl from 'mapbox-gl';

const MapboxMap = () => {
useEffect(() => {
mapboxgl.accessToken = process.env.REACT_APP_MAPBOX_API_KEY;
const map = new mapboxgl.Map({
container: 'map-container', // container id
style: 'mapbox://styles/mapbox/streets-v11', // map style
center: [-74.5, 40], // starting position [lng, lat]
zoom: 9 // starting zoom
});
}, []);

return
<div id="map-container" style={{ width: '100%', height: '500px' }} />;
};

export default MapboxMap;
