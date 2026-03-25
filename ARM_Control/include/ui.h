#pragma once
#include <Arduino.h>

const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ArmBot Control</title>
  <style>
    body { font-family: Helvetica; background: #1a1a2e; color: #eee; text-align: center; margin: 0; padding: 20px; }
    h1 { color: #e0e0e0; margin-bottom: 4px; }
    .subtitle { color: #777; font-size: 13px; margin-bottom: 30px; }
    .joint { margin: 20px auto; width: 80%; max-width: 400px; background: #16213e; border-radius: 12px; padding: 16px; }
    .joint label { display: block; font-size: 13px; color: #aaa; margin-bottom: 4px; text-transform: uppercase; letter-spacing: 1px; }
    .angle { font-size: 36px; font-weight: bold; color: #2196F3; margin-bottom: 8px; }
    input[type=range] { -webkit-appearance: none; width: 100%; height: 18px; border-radius: 9px; background: #3a3a5c; outline: none; }
    input[type=range]::-webkit-slider-thumb { -webkit-appearance: none; width: 34px; height: 34px; border-radius: 50%; background: #2196F3; cursor: pointer; }
    input[type=range]::-moz-range-thumb { width: 34px; height: 34px; border-radius: 50%; background: #2196F3; cursor: pointer; }
    .tick-row { display: flex; justify-content: space-between; color: #555; font-size: 12px; margin-top: 4px; }
    .button { background: #2196F3; border: none; color: white; padding: 12px 28px; font-size: 18px; margin: 6px; cursor: pointer; border-radius: 8px; }
    .home-btn { background: #FF5722; }
    .status { font-size: 13px; color: #555; margin-top: 20px; }
    .status.connected { color: #4CAF50; }
  </style>
</head>
<body>
  <h1>ArmBot Control</h1>
  <div class="subtitle">Connect sliders to move joints</div>

  <div class="joint">
    <label>Elbow</label>
    <div class="angle" id="elbowVal">0&deg;</div>
    <input type="range" min="0" max="180" value="0"
           oninput="send('elbow ' + this.value); document.getElementById('elbowVal').innerHTML = this.value + '&deg;'">
    <div class="tick-row"><span>0&deg;</span><span>90&deg;</span><span>180&deg;</span></div>
  </div>

  <div class="joint">
    <label>Shoulder</label>
    <div class="angle" id="shoulderVal">0&deg;</div>
    <input type="range" min="0" max="180" value="0"
           oninput="send('shoulder ' + this.value); document.getElementById('shoulderVal').innerHTML = this.value + '&deg;'">
    <div class="tick-row"><span>0&deg;</span><span>90&deg;</span><span>180&deg;</span></div>
  </div>

  <div class="joint">
    <label>Wrist</label>
    <div class="angle" id="wristVal">0&deg;</div>
    <input type="range" min="0" max="180" value="0"
           oninput="send('wrist ' + this.value); document.getElementById('wristVal').innerHTML = this.value + '&deg;'">
    <div class="tick-row"><span>0&deg;</span><span>90&deg;</span><span>180&deg;</span></div>
  </div>

  <div class="joint">
    <label>Base (steps)</label>
    <div class="angle" id="baseVal">0</div>
    <input type="range" min="-1000" max="1000" value="0"
           oninput="send('base ' + this.value); document.getElementById('baseVal').innerHTML = this.value">
    <div class="tick-row"><span>-1000</span><span>0</span><span>1000</span></div>
  </div>

  <div class="joint">
    <label>Gripper (steps)</label>
    <div class="angle" id="gripperVal">0</div>
    <input type="range" min="0" max="500" value="0"
           oninput="send('gripper ' + this.value); document.getElementById('gripperVal').innerHTML = this.value">
    <div class="tick-row"><span>0</span><span>250</span><span>500</span></div>
  </div>

  <p><button class="button home-btn" onclick="send('home')">Home All</button></p>

  <div class="status" id="status">Connecting...</div>

  <script>
    const ws = new WebSocket('ws://' + location.hostname + '/ws');
    ws.onopen = () => {
      document.getElementById('status').textContent = 'Connected';
      document.getElementById('status').className = 'status connected';
    };
    ws.onclose = () => {
      document.getElementById('status').textContent = 'Disconnected — reload to reconnect';
      document.getElementById('status').className = 'status';
    };
    ws.onmessage = (event) => {
    if (event.data === "reset") {
        // Reset all sliders to 0
        document.querySelectorAll('input[type=range]').forEach(s => s.value = 0);
        // Reset all displays
        document.getElementById('elbowVal').innerHTML    = '0&deg;';
        document.getElementById('shoulderVal').innerHTML = '0&deg;';
        document.getElementById('wristVal').innerHTML    = '0&deg;';
        document.getElementById('baseVal').innerHTML     = '0';
        document.getElementById('gripperVal').innerHTML  = '0';
        }
    };
    let debounceTimer;
    function send(msg) {
      clearTimeout(debounceTimer);
      debounceTimer = setTimeout(() => {
        if (ws.readyState === WebSocket.OPEN) ws.send(msg);
      }, 50);
      
    }
  </script>
</body>
</html>
)rawliteral";