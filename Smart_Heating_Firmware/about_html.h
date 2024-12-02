#pragma once
#include <Arduino.h>

const char* about_html = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <link rel="stylesheet" type="text/css" href="styles.css" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>Smart Heating</title>
    <link
      rel="icon"
      type="image/svg+xml"
      href="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAhsAAAIbCAYAAABCJ1y9AAAACXBIWXMAAAsTAAALEwEAmpwYAAAGQ2lUWHRYTUw6Y29tLmFkb2JlLnhtcAAAAAAAPD94cGFja2V0IGJlZ2luPSLvu78iIGlkPSJXNU0wTXBDZWhpSHpyZVN6TlRjemtjOWQiPz4gPHg6eG1wbWV0YSB4bWxuczp4PSJhZG9iZTpuczptZXRhLyIgeDp4bXB0az0iQWRvYmUgWE1QIENvcmUgNi4wLWMwMDIgNzkuMTY0NDYwLCAyMDIwLzA1LzEyLTE2OjA0OjE3ICAgICAgICAiPiA8cmRmOlJERiB4bWxuczpyZGY9Imh0dHA6Ly93d3cudzMub3JnLzE5OTkvMDIvMjItcmRmLXN5bnRheC1ucyMiPiA8cmRmOkRlc2NyaXB0aW9uIHJkZjphYm91dD0iIiB4bWxuczp4bXA9Imh0dHA6Ly9ucy5hZG9iZS5jb20veGFwLzEuMC8iIHhtbG5zOmRjPSJodHRwOi8vcHVybC5vcmcvZGMvZWxlbWVudHMvMS4xLyIgeG1sbnM6cGhvdG9zaG9wPSJodHRwOi8vbnMuYWRvYmUuY29tL3Bob3Rvc2hvcC8xLjAvIiB4bWxuczp4bXBNTT0iaHR0cDovL25zLmFkb2JlLmNvbS94YXAvMS4wL21tLyIgeG1sbnM6c3RFdnQ9Imh0dHA6Ly9ucy5hZG9iZS5jb20veGFwLzEuMC9zVHlwZS9SZXNvdXJjZUV2ZW50IyIgeG1wOkNyZWF0b3JUb29sPSJBZG9iZSBQaG90b3Nob3AgMjEuMiAoV2luZG93cykiIHhtcDpDcmVhdGVEYXRlPSIyMDI0LTA2LTI4VDIyOjQxOjQ2KzAxOjAwIiB4bXA6TW9kaWZ5RGF0ZT0iMjAyNC0wNi0yOFQyMjo1MDowNCswMTowMCIgeG1wOk1ldGFkYXRhRGF0ZT0iMjAyNC0wNi0yOFQyMjo1MDowNCswMTowMCIgZGM6Zm9ybWF0PSJpbWFnZS9wbmciIHBob3Rvc2hvcDpDb2xvck1vZGU9IjMiIHBob3Rvc2hvcDpJQ0NQcm9maWxlPSJzUkdCIElFQzYxOTY2LTIuMSIgeG1wTU06SW5zdGFuY2VJRD0ieG1wLmlpZDo1MDVhMTAzMS0xODMxLTIwNDYtODM3MC0yYjNjZDhhMWE3MTEiIHhtcE1NOkRvY3VtZW50SUQ9ImFkb2JlOmRvY2lkOnBob3Rvc2hvcDpiOTFiZDhmMi1kYzExLWJjNDUtOGNlYi02NmU1MDQ2MDAzOTYiIHhtcE1NOk9yaWdpbmFsRG9jdW1lbnRJRD0ieG1wLmRpZDozZmMyZTk2Ny02YTMyLTJiNDItYWZlYS03MzZlM2UzOGI3OWYiPiA8eG1wTU06SGlzdG9yeT4gPHJkZjpTZXE+IDxyZGY6bGkgc3RFdnQ6YWN0aW9uPSJjcmVhdGVkIiBzdEV2dDppbnN0YW5jZUlEPSJ4bXAuaWlkOjNmYzJlOTY3LTZhMzItMmI0Mi1hZmVhLTczNmUzZTM4Yjc5ZiIgc3RFdnQ6d2hlbj0iMjAyNC0wNi0yOFQyMjo0MTo0NiswMTowMCIgc3RFdnQ6c29mdHdhcmVBZ2VudD0iQWRvYmUgUGhvdG9zaG9wIDIxLjIgKFdpbmRvd3MpIi8+IDxyZGY6bGkgc3RFdnQ6YWN0aW9uPSJjb252ZXJ0ZWQiIHN0RXZ0OnBhcmFtZXRlcnM9ImZyb20gaW1hZ2UvYm1wIHRvIGltYWdlL3BuZyIvPiA8cmRmOmxpIHN0RXZ0OmFjdGlvbj0ic2F2ZWQiIHN0RXZ0Omluc3RhbmNlSUQ9InhtcC5paWQ6NTA1YTEwMzEtMTgzMS0yMDQ2LTgzNzAtMmIzY2Q4YTFhNzExIiBzdEV2dDp3aGVuPSIyMDI0LTA2LTI4VDIyOjUwOjA0KzAxOjAwIiBzdEV2dDpzb2Z0d2FyZUFnZW50PSJBZG9iZSBQaG90b3Nob3AgMjEuMiAoV2luZG93cykiIHN0RXZ0OmNoYW5nZWQ9Ii8iLz4gPC9yZGY6U2VxPiA8L3htcE1NOkhpc3Rvcnk+IDwvcmRmOkRlc2NyaXB0aW9uPiA8L3JkZjpSREY+IDwveDp4bXBtZXRhPiA8P3hwYWNrZXQgZW5kPSJyIj8+UBwXdAAAFU1JREFUeNrt3d+LXWe9x/HPjpN2C7kYEJw5iAFrkepVBO2JpxhSMlRbBK+V9m/p5N/wtqU3niNWpNVji6UiRCsYENSL4EVPj50KliBBd5NJthfrGTpN9uzM/rHWXj9eLygzSdvJ5PusvZ73XvvHjKbTaQAA6nLGCAAAsQEAiA0A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"
    />
    <link rel="preconnect" href="https://fonts.googleapis.com" />
    <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin />
    <link
      href="https://fonts.googleapis.com/css2?family=Open+Sans:ital,wght@0,300..800;1,300..800&family=Roboto:ital,wght@0,100;0,300;0,400;0,500;0,700;0,900;1,100;1,300;1,400;1,500;1,700;1,900&display=swap"
      rel="stylesheet"
    />
  </head>
  <body class="">
    <header>
      <div id="homeLink">
        <img
          id="headerLogo"
          src="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAhsAAAIbCAYAAABCJ1y9AAAACXBIWXMAAAsTAAALEwEAmpwYAAAGQ2lUWHRYTUw6Y29tLmFkb2JlLnhtcAAAAAAAPD94cGFja2V0IGJlZ2luPSLvu78iIGlkPSJXNU0wTXBDZWhpSHpyZVN6TlRjemtjOWQiPz4gPHg6eG1wbWV0YSB4bWxuczp4PSJhZG9iZTpuczptZXRhLyIgeDp4bXB0az0iQWRvYmUgWE1QIENvcmUgNi4wLWMwMDIgNzkuMTY0NDYwLCAyMDIwLzA1LzEyLTE2OjA0OjE3ICAgICAgICAiPiA8cmRmOlJERiB4bWxuczpyZGY9Imh0dHA6Ly93d3cudzMub3JnLzE5OTkvMDIvMjItcmRmLXN5bnRheC1ucyMiPiA8cmRmOkRlc2NyaXB0aW9uIHJkZjphYm91dD0iIiB4bWxuczp4bXA9Imh0dHA6Ly9ucy5hZG9iZS5jb20veGFwLzEuMC8iIHhtbG5zOmRjPSJodHRwOi8vcHVybC5vcmcvZGMvZWxlbWVudHMvMS4xLyIgeG1sbnM6cGhvdG9zaG9wPSJodHRwOi8vbnMuYWRvYmUuY29tL3Bob3Rvc2hvcC8xLjAvIiB4bWxuczp4bXBNTT0iaHR0cDovL25zLmFkb2JlLmNvbS94YXAvMS4wL21tLyIgeG1sbnM6c3RFdnQ9Imh0dHA6Ly9ucy5hZG9iZS5jb20veGFwLzEuMC9zVHlwZS9SZXNvdXJjZUV2ZW50IyIgeG1wOkNyZWF0b3JUb29sPSJBZG9iZSBQaG90b3Nob3AgMjEuMiAoV2luZG93cykiIHhtcDpDcmVhdGVEYXRlPSIyMDI0LTA2LTI4VDIyOjQxOjQ2KzAxOjAwIiB4bXA6TW9kaWZ5RGF0ZT0iMjAyNC0wNi0yOFQyMjo1MDowNCswMTowMCIgeG1wOk1ldGFkYXRhRGF0ZT0iMjAyNC0wNi0yOFQyMjo1MDowNCswMTowMCIgZGM6Zm9ybWF0PSJpbWFnZS9wbmciIHBob3Rvc2hvcDpDb2xvck1vZGU9IjMiIHBob3Rvc2hvcDpJQ0NQcm9maWxlPSJzUkdCIElFQzYxOTY2LTIuMSIgeG1wTU06SW5zdGFuY2VJRD0ieG1wLmlpZDo1MDVhMTAzMS0xODMxLTIwNDYtODM3MC0yYjNjZDhhMWE3MTEiIHhtcE1NOkRvY3VtZW50SUQ9ImFkb2JlOmRvY2lkOnBob3Rvc2hvcDpiOTFiZDhmMi1kYzExLWJjNDUtOGNlYi02NmU1MDQ2MDAzOTYiIHhtcE1NOk9yaWdpbmFsRG9jdW1lbnRJRD0ieG1wLmRpZDozZmMyZTk2Ny02YTMyLTJiNDItYWZlYS03MzZlM2UzOGI3OWYiPiA8eG1wTU06SGlzdG9yeT4gPHJkZjpTZXE+IDxyZGY6bGkgc3RFdnQ6YWN0aW9uPSJjcmVhdGVkIiBzdEV2dDppbnN0YW5jZUlEPSJ4bXAuaWlkOjNmYzJlOTY3LTZhMzItMmI0Mi1hZmVhLTczNmUzZTM4Yjc5ZiIgc3RFdnQ6d2hlbj0iMjAyNC0wNi0yOFQyMjo0MTo0NiswMTowMCIgc3RFdnQ6c29mdHdhcmVBZ2VudD0iQWRvYmUgUGhvdG9zaG9wIDIxLjIgKFdpbmRvd3MpIi8+IDxyZGY6bGkgc3RFdnQ6YWN0aW9uPSJjb252ZXJ0ZWQiIHN0RXZ0OnBhcmFtZXRlcnM9ImZyb20gaW1hZ2UvYm1wIHRvIGltYWdlL3BuZyIvPiA8cmRmOmxpIHN0RXZ0OmFjdGlvbj0ic2F2ZWQiIHN0RXZ0Omluc3RhbmNlSUQ9InhtcC5paWQ6NTA1YTEwMzEtMTgzMS0yMDQ2LTgzNzAtMmIzY2Q4YTFhNzExIiBzdEV2dDp3aGVuPSIyMDI0LTA2LTI4VDIyOjUwOjA0KzAxOjAwIiBzdEV2dDpzb2Z0d2FyZUFnZW50PSJBZG9iZSBQaG90b3Nob3AgMjEuMiAoV2luZG93cykiIHN0RXZ0OmNoYW5nZWQ9Ii8iLz4gPC9yZGY6U2VxPiA8L3htcE1NOkhpc3Rvcnk+IDwvcmRmOkRlc2NyaXB0aW9uPiA8L3JkZjpSREY+IDwveDp4bXBtZXRhPiA8P3hwYWNrZXQgZW5kPSJyIj8+UBwXdAAAFU1JREFUeNrt3d+LXWe9x/HPjpN2C7kYEJw5iAFrkepVBO2JpxhSMlRbBK+V9m/p5N/wtqU3niNWpNVji6UiRCsYENSL4EVPj50KliBBd5NJthfrGTpN9uzM/rHWXj9eLygzSdvJ5PusvZ73XvvHjKbTaQAA6nLGCAAAsQEAiA0A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"
          alt="logo"
        />
        <h1 id="headerHeading">Smart Heating</h1>
      </div>
      <div class="headerSpaser"></div>
      <label class="switch">
        <input type="checkbox" id="theme-toggle" />
        <span class="slider round"></span>
      </label>
    </header>

    <div id="mainContent">
      <nav>
        <ul>
          <li id="settings" class="navEl"><p>Settings</p></li>
          <li class="navEl">
            <a href="./controlpanel"> Control Panel </a>
          </li>
          <li class="navEl">
            <a href="./timers">Timers</a>
          </li>
          <li id="active" class="navEl last">
            <a href="./about">About</a>
          </li>
        </ul>
      </nav>

      <div id="tab" class="light-theme">
        <section id="about-section">
          <h2>About Smart Heating Controller</h2>
          <p>
            The <strong>Smart Heating Controller Attachment</strong> enhances
            your existing heating system by adding Wi-Fi functionality, enabling
            remote control, advanced timer scheduling, and robust automation
            features—all without replacing your current controller.
          </p>
          <h3>Key Features</h3>
          <ul>
            <li>
              <strong>Wi-Fi Control:</strong> Turn your heating system ON or OFF
              remotely and adjust settings via an intuitive web interface.
            </li>
            <li>
              <strong>Flexible Timer Configuration:</strong> Set timers for
              specific days or day ranges (e.g., Monday to Friday) with
              hour-specific schedules.
            </li>
            <li>
              <strong>Battery Backup:</strong> Operates on a 24-26 hour battery
              backup, ensuring continuous operation during power outages.
            </li>
            <li>
              <strong>Resilient Design:</strong> The hardware is designed as an
              add-on for existing heating controllers, equipped with a robust
              battery system.
            </li>
            <li>
              <strong>Reliable Firmware:</strong> Developed in
              Arduino-compatible C++ for performance and ease of customization.
            </li>
          </ul>
          <h3>How It Works</h3>
          <p>
            1. <strong>Installation:</strong> Attach the module to your existing
            heating controller.<br />
            2. <strong>Setup:</strong> Connect to your Wi-Fi network and access
            the web interface.<br />
            3. <strong>Operation:</strong> Configure timers, toggle heating, and
            let the system do the rest.
          </p>
          <p>
            Want to learn more or contribute? Check out our project repository:
            <a href="https://github.com/I2oman/Smart_Heating" target="_blank">
              Smart Heating Controller on GitHub </a
            >.
          </p>
          <div id="fun-fact">
            <em>Fun Fact:</em> The Smart Heating Controller stores your settings
            in its memory, so they remain intact even after a restart or power
            outage!
          </div>
        </section>
      </div>
    </div>

    <div id="slideMenuHolder" class="">
      <div id="slideMenu" class="">
        <div class="slideMenuNav">
          <ul>
            <li id="settings" class="slideNavEl"><p>Settings</p></li>
            <li class="slideNavEl">
              <a href="./controlpanel">Control Panel</a>
            </li>
            <li class="slideNavEl">
              <a href="./timers">Timers</a>
            </li>
            <li id="active" class="slideNavEl last">
              <a href="./about">About</a>
            </li>
          </ul>
        </div>
      </div>
    </div>

    <footer>
      <div>
        <p id="date"></p>
        <p id="time"></p>
      </div>
      <p>Created by Hrynchak R.</p>
    </footer>

    <div id="actionPendingCover" class="disable">
      <div class="hoverWindow">
        <div class="hoverWindowContent">
          <div class="loader"></div>
          <h1>Action Pending</h1>
        </div>
      </div>
    </div>

    <script src="script.js"></script>
  </body>
</html>
)rawliteral";
