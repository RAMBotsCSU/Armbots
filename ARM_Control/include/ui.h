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

    /* ── Servo joints (vertical sliders) ── */
    .sliders-row { display: flex; justify-content: center; gap: 16px; flex-wrap: wrap; margin-bottom: 24px; }
    .joint { display: flex; flex-direction: column; align-items: center; background: #16213e; border-radius: 12px; padding: 16px 12px; width: 80px; }
    .joint label { font-size: 11px; color: #aaa; text-transform: uppercase; letter-spacing: 1px; margin-bottom: 8px; }
    .angle { font-size: 20px; font-weight: bold; color: #2196F3; margin-bottom: 10px; }
    .slider-inner { display: flex; align-items: center; }
    .slider-wrap { display: flex; flex-direction: column; align-items: center; height: 180px; justify-content: center; }
    input[type=range][orient=vertical],
    input[type=range].vertical {
      -webkit-appearance: slider-vertical;
      writing-mode: vertical-lr;
      direction: rtl;
      width: 36px;
      height: 160px;
      cursor: pointer;
      accent-color: #2196F3;
    }
    .tick-col { display: flex; flex-direction: column; justify-content: space-between; height: 160px; color: #555; font-size: 10px; margin-left: 4px; }

    /* ── Stepper joints (horizontal sliders) ── */
    .stepper-row { display: flex; justify-content: center; gap: 16px; flex-wrap: wrap; margin-bottom: 16px; }
    .stepper-joint { display: flex; flex-direction: column; align-items: center; background: #16213e; border-radius: 12px; padding: 16px 12px; width: 140px; }
    input[type=range].horiz { width: 100%; accent-color: #2196F3; cursor: pointer; }
    .tick-row { display: flex; justify-content: space-between; color: #555; font-size: 10px; margin-top: 4px; width: 100%; }

    .button { background: #2196F3; border: none; color: white; padding: 12px 28px; font-size: 18px; margin: 6px; cursor: pointer; border-radius: 8px; }
    .home-btn { background: #FF5722; }
    .status { font-size: 13px; color: #555; margin-top: 20px; }
    .status.connected { color: #4CAF50; }
  </style>
</head>
<body>
  <h1>ArmBot Control</h1>
  <div class="subtitle">Move sliders to control joints</div>

  <!-- Servo joints — vertical sliders -->
  <div class="sliders-row">

    <div class="joint">
      <label>Elbow</label>
      <div class="angle" id="elbowVal">15&deg;</div>
      <div class="slider-inner">
        <div class="slider-wrap">
          <input type="range" class="vertical" orient="vertical"
                 min="15" max="180" value="15" step="1"
                 oninput="send('elbow ' + this.value); document.getElementById('elbowVal').innerHTML = this.value + '&deg;'">
        </div>
        <div class="tick-col"><span>180&deg;</span><span>90&deg;</span><span>15&deg;</span></div>
      </div>
    </div>

    <div class="joint">
      <label>Shoulder</label>
      <div class="angle" id="shoulderVal">0&deg;</div>
      <div class="slider-inner">
        <div class="slider-wrap">
          <input type="range" class="vertical" orient="vertical"
                 min="0" max="180" value="0" step="1"
                 oninput="send('shoulder ' + this.value); document.getElementById('shoulderVal').innerHTML = this.value + '&deg;'">
        </div>
        <div class="tick-col"><span>180&deg;</span><span>90&deg;</span><span>0&deg;</span></div>
      </div>
    </div>

    <div class="joint">
      <label>Wrist</label>
      <div class="angle" id="wristVal">22&deg;</div>
      <div class="slider-inner">
        <div class="slider-wrap">
          <input type="range" class="vertical" orient="vertical"
                 min="22.5" max="180" value="0" step="1"
                 oninput="send('wrist ' + this.value); document.getElementById('wristVal').innerHTML = this.value + '&deg;'">
        </div>
        <div class="tick-col"><span>180&deg;</span><span>90&deg;</span><span>0&deg;</span></div>
      </div>
    </div>

  </div>

  <!-- Stepper joints — horizontal sliders -->
  <div class="stepper-row">

    <div class="stepper-joint">
      <label>Base</label>
      <div class="angle" id="baseVal">0&deg;</div>
      <input type="range" class="horiz" min="-180" max="180" value="0" step="1"
              oninput="send('baseAngle ' + this.value); document.getElementById('baseVal').innerHTML = this.value + '&deg;'">
      <div class="tick-row"><span>-180&deg;</span><span>0&deg;</span><span>180&deg;</span></div>
    </div>

    <div class="stepper-joint">
      <label>Gripper</label>
      <div class="angle" id="gripperVal">0&deg;</div>
      <input type="range" class="horiz" min="0" max="180" value="0" step="1"
            oninput="send('gripperAngle ' + this.value); document.getElementById('gripperVal').innerHTML = this.value + '&deg;'">
      <div class="tick-row"><span>0&deg;</span><span>90&deg;</span><span>180&deg;</span></div>
    </div>

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
        document.querySelector('#elbowVal').innerHTML    = '15&deg;';
        document.querySelector('#shoulderVal').innerHTML = '0&deg;';
        document.querySelector('#wristVal').innerHTML    = '0&deg;';
        document.querySelector('#baseVal').innerHTML    = '0&deg;';
        document.querySelector('#gripperVal').innerHTML = '0&deg;';
        const ranges = document.querySelectorAll('input[type=range]');
        ranges[0].value = 15;  // elbow — min is 15
        ranges[1].value = 0;
        ranges[2].value = 0;
        ranges[3].value = 0;
        ranges[4].value = 0;
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