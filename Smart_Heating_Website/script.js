const timersArray = [];

class HeatingTimer {
  constructor(id = 0, startTime = "00:00", endTime = "23:59") {
    this.id = id;
    this.startTime = startTime;
    this.endTime = endTime;
    this.createElement(startTime, endTime);
    this.deleted = false;
  }

  createElement(startTime, endTime) {
    this.timeRange = document.createElement("div");
    this.timeRange.classList.add("time-range");

    this.timePeriod = document.createElement("div");
    this.timePeriod.classList.add("timePeriod");

    this.startTimeContainer = document.createElement("div");
    this.startTimeContainer.classList.add("timeContainer");
    this.startTimeLabel = document.createElement("label");
    this.startTimeLabel.textContent = "Start Time";
    this.startTimeInput = document.createElement("input");
    this.startTimeInput.type = "time";
    this.startTimeInput.name = "start-time";
    this.startTimeInput.value = startTime;
    this.startTimeContainer.appendChild(this.startTimeLabel);
    this.startTimeContainer.appendChild(this.startTimeInput);

    this.endTimeContainer = document.createElement("div");
    this.endTimeContainer.classList.add("timeContainer");
    this.endTimeLabel = document.createElement("label");
    this.endTimeLabel.textContent = "End Time";
    this.endTimeInput = document.createElement("input");
    this.endTimeInput.type = "time";
    this.endTimeInput.name = "end-time";
    this.endTimeInput.value = endTime;
    this.endTimeContainer.appendChild(this.endTimeLabel);
    this.endTimeContainer.appendChild(this.endTimeInput);

    this.timePeriod.appendChild(this.startTimeContainer);
    this.timePeriod.appendChild(this.endTimeContainer);

    this.removeButton = document.createElement("button");
    this.removeButton.textContent = "Remove Time Period";
    this.removeButton.addEventListener("click", () => {
      this.timeRange.remove();
      this.deleted = true;
    });

    this.timeRange.appendChild(this.timePeriod);
    this.timeRange.appendChild(this.removeButton);
  }
}

class DayTimer {
  constructor(id = 0, startDay = 0, endDay = 6) {
    this.deleted = false;

    this.id = id;
    this.startDay = startDay;
    this.endDay = endDay;

    this.timeRangesArray = [];

    this.createElement(startDay, endDay);
  }

  createElement(startDay, endDay) {
    this.element = document.createElement("div");
    this.element.classList.add("timer-item");

    this.timerItemHeader = document.createElement("div");
    this.timerItemHeader.classList.add("timer-item-header");

    this.timePeriod = document.createElement("div");
    this.timePeriod.classList.add("timePeriod");

    this.startDayContainer = document.createElement("div");
    this.startDaylabel = document.createElement("label");
    this.startDaylabel.textContent = "Start Day";
    this.startDaySelect = this.createDaySelect("start-day");
    this.startDaySelect.value = startDay;
    this.startDayContainer.appendChild(this.startDaylabel);
    this.startDayContainer.appendChild(this.startDaySelect);

    this.endDayContainer = document.createElement("div");
    this.endDaylabel = document.createElement("label");
    this.endDaylabel.textContent = "End Day";
    this.endDaySelect = this.createDaySelect("end-day");
    this.endDaySelect.value = endDay;
    this.endDayContainer.appendChild(this.endDaylabel);
    this.endDayContainer.appendChild(this.endDaySelect);

    this.timePeriod.appendChild(this.startDayContainer);
    this.timePeriod.appendChild(this.endDayContainer);

    this.timeRangesContainer = document.createElement("div");
    this.timeRangesContainer.classList.add("time-ranges");

    this.addTimerButton = document.createElement("button");
    this.addTimerButton.textContent = "Add Time Period";
    this.addTimerButton.addEventListener("click", () => {
      this.timeRangesArray.push(new HeatingTimer());
      this.refreshPage();
    });

    this.timerItemHeader.appendChild(this.timePeriod);
    this.timerItemHeader.appendChild(this.addTimerButton);

    this.element.appendChild(this.timerItemHeader);
    this.element.appendChild(this.timeRangesContainer);

    this.timerButtons = document.createElement("div");
    this.timerButtons.classList.add("timer-buttons");

    this.updateButton = document.createElement("button");
    this.updateButton.textContent = "Update Timer";
    this.updateButton.addEventListener("click", () => {
      this.updateTimer();
    });
    this.turnOff(this.updateButton);

    this.editButton = document.createElement("button");
    this.editButton.textContent = "Edit Timer";
    this.editButton.addEventListener("click", () => {
      this.editTimer();
    });
    this.turnOff(this.editButton);

    this.deleteButton = document.createElement("button");
    this.deleteButton.textContent = "Delete";
    this.deleteButton.id = "delete";
    this.deleteButton.addEventListener("click", () => {
      this.deleteTimer();
    });
    this.turnOff(this.deleteButton);

    this.setButton = document.createElement("button");
    this.setButton.textContent = "Set Timer";
    this.setButton.addEventListener("click", () => {
      this.setTimer();
    });

    this.cancelButton = document.createElement("button");
    this.cancelButton.textContent = "Cancel";
    this.cancelButton.addEventListener("click", () => {
      this.cancelTimer();
    });

    this.timerButtons.appendChild(this.updateButton);
    this.timerButtons.appendChild(this.editButton);
    this.timerButtons.appendChild(this.deleteButton);
    this.timerButtons.appendChild(this.setButton);
    this.timerButtons.appendChild(this.cancelButton);

    this.element.appendChild(this.timerButtons);
  }

  createDayContainer(labelText, name) {
    const container = document.createElement("div");
    const label = document.createElement("label");
    label.textContent = labelText;
    const select = this.createDaySelect(name);
    container.appendChild(label);
    container.appendChild(select);
    return container;
  }

  createDaySelect(name) {
    const daysOfWeek = [
      "Monday",
      "Tuesday",
      "Wednesday",
      "Thursday",
      "Friday",
      "Saturday",
      "Sunday",
    ];

    const select = document.createElement("select");
    select.name = name;

    daysOfWeek.forEach((day) => {
      const option = document.createElement("option");
      option.value = daysOfWeek.indexOf(day);
      option.textContent = day;
      select.appendChild(option);
    });

    return select;
  }

  updateTimer() {
    let invalidInput = false;

    const modifyDaysArray = {};

    if (
      this.startDaySelect.value != this.startDay ||
      this.endDaySelect.value != this.endDay
    ) {
      if (this.startDaySelect.value > this.endDaySelect.value) {
        invalidInput = true;
        alert("Wrong days selection!");
        return;
      }

      if (
        !this.isValidDaysPeriod(
          this.startDaySelect.value,
          this.endDaySelect.value
        )
      ) {
        invalidInput = true;
        alert("Timer intercepts with previous!");
        return;
      }

      modifyDaysArray["startDay"] = this.startDaySelect.value;
      modifyDaysArray["endDay"] = this.endDaySelect.value;
    }

    const addTimeRangesArray = [];
    const modifyTimeRangesArray = [];
    const deleteTimeRangesArray = [];

    if (this.timeRangesArray.length == 0) {
      invalidInput = true;
      alert("Add at least 1 time period");
      return;
    }

    this.timeRangesArray.forEach((timeRange) => {
      if (timeRange.deleted) {
        if (timeRange.id != 0) {
          deleteTimeRangesArray.push(timeRange.id);
        }
        return;
      }

      const [startHours, startMinutes] = timeRange.startTimeInput.value
        .split(":")
        .map(Number);
      const [endHours, endMinutes] = timeRange.endTimeInput.value
        .split(":")
        .map(Number);
      if (startHours * 60 + startMinutes >= endHours * 60 + endMinutes) {
        invalidInput = true;
        alert(
          "Wrong " +
            (this.timeRangesArray.indexOf(timeRange) + 1) +
            " time selection!"
        );
        return;
      }

      if (
        !this.isValidTimePeriod(
          timeRange.startTimeInput.value,
          timeRange.endTimeInput.value
        )
      ) {
        invalidInput = true;
        alert(
          "Time period " +
            (this.timeRangesArray.indexOf(timeRange) + 1) +
            " intercepts with previous!"
        );
        return;
      }

      const timeRangeJson = {
        startTime: {
          hours: startHours,
          minutes: startMinutes,
        },
        endTime: {
          hours: endHours,
          minutes: endMinutes,
        },
      };

      if (timeRange.id == 0) {
        addTimeRangesArray.push(timeRangeJson);
      } else {
        if (
          timeRange.startTimeInput.value != timeRange.startTime ||
          timeRange.endTimeInput.value != timeRange.endTime
        ) {
          const mTimeRangeJson = {
            timerId: timeRange.id,
            timer: timeRangeJson,
          };
          modifyTimeRangesArray.push(mTimeRangeJson);
        }
      }

      timeRange.startTime = timeRange.startTimeInput.value;
      timeRange.endTime = timeRange.endTimeInput.value;
    });

    if (invalidInput) return;

    this.startDay = this.startDaySelect.value;
    this.endDay = this.endDaySelect.value;

    const modifyDayTimerJson = {
      action: "modifyDayTimer",
      dayTimerId: this.id,
      changes: [],
    };
    if (Object.keys(modifyDaysArray).length > 0) {
      modifyDayTimerJson["changes"].push({
        action: "modifyDays",
        dayTimer: modifyDaysArray,
      });
    }
    if (addTimeRangesArray.length > 0) {
      modifyDayTimerJson["changes"].push({
        action: "addTimers",
        timers: addTimeRangesArray,
      });
    }
    if (modifyTimeRangesArray.length > 0) {
      modifyDayTimerJson["changes"].push({
        action: "modifyTimers",
        timers: modifyTimeRangesArray,
      });
    }
    if (deleteTimeRangesArray.length > 0) {
      modifyDayTimerJson["changes"].push({
        action: "deleteTimers",
        timerIds: deleteTimeRangesArray,
      });
    }
    this.doneConfiguring();
    // contentLoading();

    console.log(modifyDayTimerJson);
    // TODO: turn on message sent
    // websocket.send(JSON.stringify(modifyDayTimerJson));
  }

  editTimer() {
    this.turnOn(this.addTimerButton);

    this.startDaySelect.style.pointerEvents = "";
    this.startDaySelect.style.color = "";
    this.endDaySelect.style.pointerEvents = "";
    this.endDaySelect.style.color = "";

    this.timeRangesArray.forEach((timeRange) => {
      timeRange.startTimeInput.readOnly = false;
      timeRange.startTimeInput.style.color = "";
      timeRange.endTimeInput.readOnly = false;
      timeRange.endTimeInput.style.color = "";

      this.turnOn(timeRange.removeButton);
    });
    this.turnOn(this.updateButton);
    this.turnOff(this.editButton);
    this.turnOn(this.deleteButton);
    this.turnOff(this.setButton);
    this.turnOn(this.cancelButton);
  }

  deleteTimer() {
    const deleteDayTimer = {
      action: "deleteDayTimer",
      dayTimerId: this.id,
    };
    console.log(deleteDayTimer);
    // TODO: turn on message sent
    // websocket.send(JSON.stringify(deleteDayTimer));
    this.doneConfiguring();
    // contentLoading();
    this.deleteItself();
  }

  setTimer() {
    let invalidInput = false;

    const addDayTimer = {
      action: "addDayTimer",
    };

    const dayTimerJson = {};

    if (this.startDaySelect.value > this.endDaySelect.value) {
      invalidInput = true;
      alert("Wrong days selection!");
      return;
    }

    if (
      !this.isValidDaysPeriod(
        this.startDaySelect.value,
        this.endDaySelect.value
      )
    ) {
      invalidInput = true;
      alert("Timer intercepts with previous!");
      return;
    }

    dayTimerJson["startDay"] = this.startDaySelect.value;
    dayTimerJson["endDay"] = this.endDaySelect.value;

    const timeRangesArray = [];

    if (this.timeRangesArray.length == 0) {
      invalidInput = true;
      alert("Add at least 1 time period");
      return;
    }

    this.timeRangesArray.forEach((timeRange) => {
      if (timeRange.deleted) {
        return;
      }

      const [startHours, startMinutes] = timeRange.startTimeInput.value
        .split(":")
        .map(Number);
      const [endHours, endMinutes] = timeRange.endTimeInput.value
        .split(":")
        .map(Number);
      if (startHours * 60 + startMinutes >= endHours * 60 + endMinutes) {
        invalidInput = true;
        alert(
          "Wrong " +
            (this.timeRangesArray.indexOf(timeRange) + 1) +
            " time selection!"
        );
        return;
      }

      if (
        !this.isValidTimePeriod(
          timeRange.startTimeInput.value,
          timeRange.endTimeInput.value
        )
      ) {
        invalidInput = true;
        alert(
          "Time period " +
            (this.timeRangesArray.indexOf(timeRange) + 1) +
            " intercepts with previous!"
        );
        return;
      }

      const timeRangeJson = {
        startTime: {
          hours: startHours,
          minutes: startMinutes,
        },
        endTime: {
          hours: endHours,
          minutes: endMinutes,
        },
      };

      timeRangesArray.push(timeRangeJson);

      timeRange.startTime = timeRange.startTimeInput.value;
      timeRange.endTime = timeRange.endTimeInput.value;
    });

    if (invalidInput) return;

    this.startDay = this.startDaySelect.value;
    this.endDay = this.endDaySelect.value;

    dayTimerJson["timers"] = timeRangesArray;

    addDayTimer["dayTimer"] = dayTimerJson;

    this.doneConfiguring();
    // contentLoading();

    console.log(addDayTimer);
    // TODO: turn on message sent
    // websocket.send(JSON.stringify(addDayTimer));
  }

  cancelTimer() {
    if (this.id == 0) {
      this.deleteItself();
    } else {
      this.startDaySelect.value = this.startDay;
      this.endDaySelect.value = this.endDay;

      this.timeRangesArray.forEach((timeRange) => {
        timeRange.startTimeInput.value = timeRange.startTime;
        timeRange.endTimeInput.value = timeRange.endTime;
      });

      this.doneConfiguring();
    }
  }

  doneConfiguring() {
    this.turnOff(this.addTimerButton);

    this.startDaySelect.style.pointerEvents = "none";
    this.startDaySelect.style.color = "rgb(169, 169, 169)";
    this.endDaySelect.style.pointerEvents = "none";
    this.endDaySelect.style.color = "rgb(169, 169, 169)";

    this.timeRangesArray.forEach((timeRange) => {
      timeRange.startTimeInput.readOnly = true;
      timeRange.startTimeInput.style.color = "rgb(169, 169, 169)";
      timeRange.endTimeInput.readOnly = true;
      timeRange.endTimeInput.style.color = "rgb(169, 169, 169)";

      this.turnOff(timeRange.removeButton);
    });
    this.turnOff(this.updateButton);
    this.turnOn(this.editButton);
    this.turnOn(this.deleteButton);
    this.turnOff(this.setButton);
    this.turnOff(this.cancelButton);

    // FIXME: moved to contentLoading()
  }

  turnOn(button) {
    button.className = "";
  }

  turnOff(button) {
    button.className = "";
    button.classList.add("disabled");
  }

  isValidDaysPeriod(startDay, endDay) {
    let invalidDays = {};

    timersArray.forEach((timer) => {
      if (timer.id != this.id) {
        for (let i = timer.startDay; i <= timer.endDay; i++) {
          invalidDays[i] = true;
        }
      }
    });

    for (let i = startDay; i <= endDay; i++) {
      if (invalidDays[i]) return false;
    }

    return true;
  }

  isValidTimePeriod(startTime, endTime) {
    let invalidTimes = {};

    let c = 0;

    this.timeRangesArray.forEach((timeRange) => {
      if (
        timeRange.startTimeInput.value == startTime &&
        timeRange.endTimeInput.value == endTime
      ) {
        c += 1;
      } else {
        const [startHours, startMinutes] = timeRange.startTimeInput.value
          .split(":")
          .map(Number);
        const [endHours, endMinutes] = timeRange.endTimeInput.value
          .split(":")
          .map(Number);
        for (
          let i = startHours * 60 + startMinutes;
          i <= endHours * 60 + endMinutes;
          i++
        ) {
          invalidTimes[i] = true;
        }
      }
    });

    if (c == 2) {
      return false;
    }

    const [startHours, startMinutes] = startTime.split(":").map(Number);
    const [endHours, endMinutes] = endTime.split(":").map(Number);
    for (
      let i = startHours * 60 + startMinutes;
      i <= endHours * 60 + endMinutes;
      i++
    ) {
      if (invalidTimes[i]) return false;
    }

    return true;
  }

  deleteItself() {
    this.deleted = true;
    this.element.remove();
    timersArray.splice(timersArray.indexOf(this), 1);
    this.refreshPage();
  }

  refreshPage() {
    this.timeRangesContainer.innerHTML = "";
    this.timeRangesArray.forEach((range) => {
      if (!range.deleted) {
        this.timeRangesContainer.appendChild(range.timeRange);
      }
    });
  }
}

// var gateway = `ws://${window.location.hostname}/ws`;
var gateway = `ws://192.168.0.225/ws`;
var websocket;

// window.addEventListener("load", onLoad);

function onLoad(event) {
  initWebSocket();
}

function initWebSocket() {
  console.log("Trying to open a WebSocket connection...");
  websocket = new WebSocket(gateway);
  websocket.onmessage = onMessage;
  websocket.onopen = onOpen;
  websocket.onclose = onClose;
}

function onOpen(event) {
  console.log("Connection opened");
  if (
    window.location.pathname === "/timers" ||
    window.location.pathname === "/timers.html" ||
    window.location.pathname === "/Smart_Heating_Website/timers" ||
    window.location.pathname === "/Smart_Heating_Website/timers.html"
  ) {
    const message = {
      action: "getDayTimers",
    };
    console.log(message);
    websocket.send(JSON.stringify(message));
  }
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

  const mainContentContainer = document.getElementById("mainContentContainer");
  const mainContentLoader = document.getElementById("mainContentLoader");

  const slideMenu = document.getElementById("slideMenu");

  mainContentContainer.className = "";
  mainContentContainer.classList.add("container");

  mainContentLoader.className = "";
  mainContentLoader.classList.add("loaderHolder");
  mainContentLoader.classList.add("disabled");

  switch (data["action"]) {
    case "init_update":
      console.log("Handling init_update");
      body.className = "";
      body.classList.add(data["heatingStatus"]);
      slideMenu.className = "";
      slideMenu.classList.add(data["heatingStatus"]);
      try {
        const modeCheckBox = document.getElementById("check");
        modeCheckBox.checked = data["heatingMode"];
      } catch (err) {
        // pass
      }
      console.log("Body class updated to:", body.className);
      document.getElementById("date").innerText = data["date"];
      document.getElementById("time").innerText = data["time"];
      break;

    case "status_update":
      actionPendingCover.className = "";
      actionPendingCover.classList.add("disable");
      body.className = "";
      body.classList.add(data["heatingStatus"]);
      slideMenu.className = "";
      slideMenu.classList.add(data["heatingStatus"]);
      break;

    case "time_update":
      document.getElementById("date").innerText = data["date"];
      document.getElementById("time").innerText = data["time"];
      break;

    case "dayTimersList_update":
      const dayTimers = data["dayTimers"];
      if (dayTimers.length == 0) return;
      timersArray.splice(0, timersArray.length);
      dayTimers.forEach((dayTimer) => {
        receivedDayTimer = new DayTimer(
          dayTimer["id"],
          dayTimer["startDay"],
          dayTimer["endDay"]
        );
        dayTimer["timers"].forEach((timer) => {
          receivedDayTimer.timeRangesArray.push(
            new HeatingTimer(
              timer["id"],
              `${String(timer["startTime"]["hours"]).padStart(2, "0")}:${String(
                timer["startTime"]["minutes"]
              ).padStart(2, "0")}`,
              `${String(timer["endTime"]["hours"]).padStart(2, "0")}:${String(
                timer["endTime"]["minutes"]
              ).padStart(2, "0")}`
            )
          );
        });
        receivedDayTimer.doneConfiguring();
        timersArray.push(receivedDayTimer);
      });

      const timerList = document.getElementById("timer-list");
      timerList.innerHTML = "";
      timersArray.forEach((timer) => {
        timer.refreshPage();
        timerList.appendChild(timer.element);
      });
      break;
  }
}

function toggleHeating() {
  const modeCheckBox = document.getElementById("check");
  if (modeCheckBox.checked) {
    const actionPendingCover = document.getElementById("actionPendingCover");
    actionPendingCover.className = "";
    actionPendingCover.classList.add("enable");
    const message = {
      action: "togleHeating",
    };
    console.log(message);
    websocket.send(JSON.stringify(message));
  } else {
    alert("Unavailable in WINTER mode");
  }
}

document.addEventListener("DOMContentLoaded", function () {
  const tab = document.getElementById("tab");
  const themeSwitch = document.getElementById("theme-toggle");

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

  //------------------------------------------------------------

  try {
    const homeLink = document.getElementById("homeLink");
    const slideMenuHolder = document.getElementById("slideMenuHolder");

    function handleMediaQueryChange(event) {
      if (event.matches) {
        homeLink.addEventListener("click", openMenu);
        slideMenuHolder.addEventListener("touchstart", closeMenu);
      } else {
        homeLink.removeEventListener("click", openMenu);
        slideMenuHolder.removeEventListener("click", closeMenu);
      }
    }

    function openMenu() {
      slideMenuHolder.className = "active";
    }

    function closeMenu(e) {
      if (!e.target.closest("#slideMenu")) {
        slideMenuHolder.className = "";
      }
    }

    const mediaQuery = window.matchMedia("(max-width: 500px)");

    handleMediaQueryChange(mediaQuery);

    mediaQuery.addEventListener("change", handleMediaQueryChange);
  } catch (err) {
    // pass
  }

  //------------------------------------------------------------

  try {
    const modeCheckBox = document.getElementById("check");
    modeCheckBox.addEventListener("change", function () {
      const message = {
        action: "switchHeatingMode",
        heatingMode: modeCheckBox.checked,
      };
      console.log(message);
      websocket.send(JSON.stringify(message));
    });
  } catch (err) {
    // pass
  }

  //------------------------------------------------------------

  try {
    const timerList = document.getElementById("timer-list");
    const addItemButton = document.getElementById("add-item");

    addItemButton.addEventListener("click", function () {
      timersArray.push(new DayTimer());
      refreshPage();
    });

    function refreshPage() {
      timerList.innerHTML = "";
      timersArray.forEach((timer) => {
        timer.refreshPage();
        if (!timer.deleted) {
          timerList.appendChild(timer.element);
        }
      });
    }

    refreshPage();
  } catch (err) {
    // pass
  }
});

function contentLoading() {
  const mainContentContainer = document.getElementById("mainContentContainer");
  const mainContentLoader = document.getElementById("mainContentLoader");
  mainContentContainer.className = "";
  mainContentContainer.classList.add("container");
  mainContentContainer.classList.add("disabled");
  mainContentLoader.className = "";
  mainContentLoader.classList.add("loaderHolder");
}

window.addEventListener("load", loadTestTimers);

function loadTestTimers(event) {
  if (
    !(
      window.location.pathname === "/timers" ||
      window.location.pathname === "/Smart_Heating_Website/timers.html"
    )
  ) {
    return;
  }
  const data = {
    winterMode: true,
    dayTimers: [
      {
        id: 1,
        days: {
          startDay: 0,
          endDay: 1,
        },
        timers: [
          {
            id: 3341131,
            startTime: {
              hours: 5,
              minutes: 45,
            },
            endTime: {
              hours: 8,
              minutes: 30,
            },
          },
          {
            id: 3341191,
            startTime: {
              hours: 18,
              minutes: 30,
            },
            endTime: {
              hours: 23,
              minutes: 0,
            },
          },
        ],
      },
      {
        id: 2,
        days: {
          startDay: 5,
          endDay: 5,
        },
        timers: [
          {
            id: 3341324,
            startTime: {
              hours: 18,
              minutes: 30,
            },
            endTime: {
              hours: 23,
              minutes: 0,
            },
          },
        ],
      },
      {
        id: 3,
        days: {
          startDay: 6,
          endDay: 6,
        },
        timers: [
          {
            id: 3341324,
            startTime: {
              hours: 18,
              minutes: 30,
            },
            endTime: {
              hours: 23,
              minutes: 0,
            },
          },
        ],
      },
    ],
  };

  const dayTimers = data["dayTimers"];
  if (dayTimers.length == 0) return;
  timersArray.splice(0, timersArray.length);
  dayTimers.forEach((dayTimer) => {
    receivedDayTimer = new DayTimer(
      dayTimer["id"],
      dayTimer["days"]["startDay"],
      dayTimer["days"]["endDay"]
    );
    dayTimer["timers"].forEach((timer) => {
      receivedDayTimer.timeRangesArray.push(
        new HeatingTimer(
          timer["id"],
          `${String(timer["startTime"]["hours"]).padStart(2, "0")}:${String(
            timer["startTime"]["minutes"]
          ).padStart(2, "0")}`,
          `${String(timer["endTime"]["hours"]).padStart(2, "0")}:${String(
            timer["endTime"]["minutes"]
          ).padStart(2, "0")}`
        )
      );
    });
    receivedDayTimer.doneConfiguring();
    timersArray.push(receivedDayTimer);
  });

  const timerList = document.getElementById("timer-list");
  timerList.innerHTML = "";
  timersArray.forEach((timer) => {
    timer.refreshPage();
    timerList.appendChild(timer.element);
  });
}

window.addEventListener("load", githubLinks);

function githubLinks() {
  const base =
    "https://htmlpreview.github.io/?https://raw.githubusercontent.com/I2oman/Smart_Heating/master/Smart_Heating_Website/";
  document.querySelectorAll("a[href]").forEach((link) => {
    if (link.getAttribute("href").startsWith("./")) {
      link.href = base + link.getAttribute("href").substring(2);
    }
  });
}
