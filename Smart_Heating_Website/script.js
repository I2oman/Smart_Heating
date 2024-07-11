document.addEventListener("DOMContentLoaded", function () {
  const themeToggleButton = document.getElementById("theme-toggle");
  const body = document.body;
  const tab = document.getElementById("tab");
  const themeSwitch = document.getElementById("theme-toggle");

  const savedBodyClass = localStorage.getItem("bodyClass");
  if (savedBodyClass) {
    body.className = "";
    body.classList.add(savedBodyClass);
  }

  const savedTheme = localStorage.getItem("theme");
  if (savedTheme) {
    tab.className = "";
    tab.classList.add(savedTheme);
    themeSwitch.checked = savedTheme === "dark-theme";
  }

  themeSwitch.addEventListener("change", function () {
    if (themeSwitch.checked) {
      tab.classList.remove("light-theme");
      tab.classList.add("dark-theme");
      localStorage.setItem("theme", "dark-theme");
    } else {
      tab.classList.remove("dark-theme");
      tab.classList.add("light-theme");
      localStorage.setItem("theme", "light-theme");
    }
  });
});

var gateway = `ws://${window.location.hostname}/ws`;
var websocket;

window.addEventListener("load", onLoad);

function onLoad(event) {
  initWebSocket();
}

function initWebSocket() {
  console.log("Trying to open a WebSocket connection...");
  websocket = new WebSocket(gateway);
  websocket.onopen = onOpen;
  websocket.onclose = onClose;
  websocket.onmessage = onMessage;
}

function onOpen(event) {
  console.log("Connection opened");
}

function onClose(event) {
  console.log("Connection closed");
  setTimeout(initWebSocket, 2000);
}

function onMessage(event) {
  const body = document.body;
  console.log("Message from server: ", event.data);
  const data = JSON.parse(event.data);
  console.log("Received JSON:", data);
  const actionPendingCover = document.getElementById("actionPendingCover");
  // Process the received JSON data

  switch (data["request_type"]) {
    case "init_update":
      for (const key in data) {
        if (key === "status_update") {
          continue;
        }
        body.className = "";
        body.classList.add(data[key]);
        localStorage.setItem("bodyClass", data[key]);
      }
      break;
    case "status_update":
      for (const key in data) {
        if (key === "status_update") {
          continue;
        }
        if (data.hasOwnProperty(key)) {
          const element = document.getElementById(key);
          if (element) {
            element.textContent = data[key];
          }
        }
        if (key === "heatingStatus") {
          body.className = "";
          body.classList.add(data[key]);
          localStorage.setItem("bodyClass", data[key]);
        }
      }
      break;
    case "toggle_response":
      actionPendingCover.className = "";
      actionPendingCover.classList.add("disable");
      for (const key in data) {
        if (key === "status_update") {
          continue;
        }
        body.className = "";
        body.classList.add(data[key]);
        localStorage.setItem("bodyClass", data[key]);
      }
      break;
  }
}

function toggleHeating() {
  const actionPendingCover = document.getElementById("actionPendingCover");
  actionPendingCover.className = "";
  actionPendingCover.classList.add("enable");
  const message = {
    toggleHeating: "1",
  };
  websocket.send(JSON.stringify(message));
}

function servoWrite() {
  const message = {
    servoWrite: document.getElementById("degreeToTurn").value,
  };
  websocket.send(JSON.stringify(message));
}
