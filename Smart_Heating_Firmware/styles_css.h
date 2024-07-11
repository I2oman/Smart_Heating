#pragma once
#include <Arduino.h>

const char* styles_css = R"rawliteral(
* {
  padding: 0px;
  margin: 0px;
  box-sizing: border-box;
}

html,
body {
  min-height: 100vh;
  width: 100vw;
  height: fit-content;
  background-color: rgb(150, 150, 150);
}

body.heatingOn {
  background-color: lightcoral;
}

body.heatingOn #heatingStatus {
  filter: opacity(1);
}

body.heatingOff {
  background-color: cornflowerblue;
}

body.heatingOff #heatingStatus {
  filter: opacity(0.375);
}

body.toggle_error #heatingStatus {
  filter: opacity(0.375) grayscale(1);
}

body.toggle_error #heatingGroup h1 {
  display: block;
  padding: 1rem;
  background-color: red;
}

body {
  display: flex;
  flex-direction: column;
}

header {
  width: 100vw;
  display: flex;
  align-items: center;
  padding: clamp(0.1rem, 0.1rem + 1vw, 0.5rem);
  height: fit-content;
}

.headerSpaser {
  flex-grow: 1;
}

#logoBtn {
  background-color: transparent;
  border: none;
}

#headerLogo {
  height: clamp(1rem, 1rem + 5vw, 5rem);
  width: clamp(1rem, 1rem + 5vw, 5rem);
}

#headerHeading {
  margin-left: clamp(0.2rem, 0.2rem + 2vw, 1rem);
  font-size: clamp(1rem, 1rem + 5vw, 5rem);
  color: white;
}

#mainContent {
  display: flex;
  width: 100vw;
  flex: 1;
}

#mainContent ul {
  list-style-type: none;
}

nav {
  display: flex;
  flex-direction: column;
  width: clamp(2rem, 5rem + 10vw, 20rem);
  margin: clamp(0.05rem, 0.05rem + 0.5vw, 0.25rem);
  margin-top: 0;
  margin-bottom: 0;
  padding: clamp(0.1rem, 0.1rem + 1vw, 0.5rem);
  padding-top: 0;
  padding-bottom: 0;
  /* width: 15vw;
  padding: 1vh; */
  border: 0.15vw solid rgba(255, 255, 255, 0.5);
  border-bottom: 0;
  border-radius: 1vw 1vw 0 0;
}

nav ul {
  display: flex;
  flex-direction: column;
  flex-grow: 1;
}

.navEl {
  border-radius: clamp(0.1rem, 0.2rem + 0.5vw, 1rem);
  margin-bottom: clamp(0.05rem, 0.05rem + 0.5vw, 0.25rem);
  transition: 0.25s;
}

.navEl p {
  display: block;
  text-align: center;
  padding: clamp(0.1rem, 0.1rem + 1vw, 0.5rem);
  padding-top: 0;
  padding-bottom: 0;
  font-size: clamp(0.7rem, 0.7rem + 1.5vw, 5rem);
  color: white;
}

.navEl a {
  display: block;
  padding: clamp(0.1rem, 0.1rem + 1vw, 0.5rem);
  font-size: clamp(0.5rem, 0.5rem + 1vw, 3rem);
  color: white;
  text-decoration: none;
}

.navEl.last {
  margin-top: auto;
}

#active {
  backdrop-filter: brightness(0.5);
}

.navEl:hover:not(#active, #settings) {
  backdrop-filter: brightness(1.5);
}

#tab {
  flex: 1;
  border: 0.1vw solid black;
  border-bottom: 0;
  border-right: 0;
  border-radius: 2vw 0 0 0;
  background-color: white;
  padding: clamp(0.1rem, 0.1rem + 1vw, 0.5rem);
}

#tab.light-theme {
  background-color: rgb(255, 255, 255);
}
#tab.light-theme .infoValue {
  color: green;
}

#tab.dark-theme {
  background-color: rgb(115, 115, 115);
}
#tab.dark-theme .infoValue {
  color: lightgreen;
}

#heatingGroup {
  display: flex;
  flex-direction: column;
  width: fit-content;
  align-items: center;
  margin-left: auto;
  margin-right: auto;
}

.fireImg {
  width: clamp(5vw, 5vw + 5vh, 15vw);
  height: auto;
  /* filter: opacity(1); */
  /* filter: opacity(0.375); */
}

#heatingBtn {
  width: fit-content;
  font-size: clamp(1rem, 0.5rem + 1vw, 3rem);
  margin: 1em;
  padding: 1em;
  background-color: rgba(255, 170, 0, 0.2);
  color: black;
  border: 2px solid rgb(255, 170, 0);
  transition-duration: 0.4s;
  border-radius: 0.75em;
}

#heatingBtn:hover {
  box-shadow: 0.25em 0.25em 16px 0 rgba(0, 0, 0, 0.2),
    -0.25em 0.25em 16px 0 rgba(0, 0, 0, 0.2);
  background-color: rgb(255, 170, 0);
  color: white;
  cursor: pointer;
}

body #heatingGroup h1 {
  /* text-align: center; */
  display: none;
}

#aboutHeading {
  flex: 1;
  text-align: center;
}

.infoDiv {
  display: flex;
  align-items: baseline;
}

.infoObj {
  flex: 1;
}

.infoLable {
  text-align: end;
}

.infoSeparator {
  margin-left: 1em;
  margin-right: 1em;
}

.infoValue {
  display: flex;
}

#degreeToTurn {
  width: 5vw;
  font-size: 1.5rem;
}

.infoObjBtn {
  width: 100%;
  display: grid;
  place-items: center;
}

#servoBtn {
  width: fit-content;
  margin: 0.5rem;
  font-size: 1.5rem;
  padding: 0.5em;
  background-color: rgba(174, 0, 255, 0.2);
  color: black;
  border: 2px solid rgb(174, 0, 255);
  transition-duration: 0.4s;
  border-radius: 0.75em;
}

#servoBtn:hover {
  box-shadow: 0.25em 0.25em 16px 0 rgba(0, 0, 0, 0.2),
    -0.25em 0.25em 16px 0 rgba(0, 0, 0, 0.2);
  background-color: rgb(174, 0, 255);
  color: white;
  cursor: pointer;
}

#notFoundDiv {
  display: flex;
  flex-direction: column;
  align-items: center;
  margin: auto;
}

#notFound404 {
  font-size: 24vw;
}

#notFoundText {
  font-size: 5vw;
}

footer {
  width: 100vw;
  padding: clamp(0.1rem, 0.1rem + 1vw, 0.5rem);
  height: fit-content;
  background-color: black;
}

footer p {
  font-size: clamp(0.5rem, 0.5rem + 1vw, 3rem);
  color: white;
  text-align: right;
}

#actionPendingCover.disable {
  position: absolute;
  width: 100vw;
  height: 100vh;
  display: none;
}

#actionPendingCover.enable {
  position: absolute;
  width: 100vw;
  height: 100vh;
  display: block;
}

.hoverWindow {
  width: 100vw;
  height: 100vh;
  background-color: rgba(128, 128, 128, 0.75);
  display: grid;
  place-items: center;
}

.hoverWindowContent {
  display: flex;
  flex-direction: column;
  width: fit-content;
  height: fit-content;
  align-items: center;
  filter: drop-shadow(0 0 5px rgba(0, 0, 0, 0.5));
}

.loader {
  width: 10em;
  aspect-ratio: 1.154;
  --_g: no-repeat radial-gradient(farthest-side, white 90%, #0000);
  background: var(--_g) 50% 0, var(--_g) 0 100%, var(--_g) 100% 100%;
  background-size: 35% calc(35% * 1.154);
  animation: l16 1s infinite;
}

@keyframes l16 {
  50%,
  100% {
    background-position: 100% 100%, 50% 0, 0 100%;
  }
}

.hoverWindowContent h1 {
  margin-top: 10vh;
  color: white;
  font-size: clamp(1rem, 1rem + 7vw, 7rem);
}

/* Toggle switch styling */
.switch {
  position: relative;
  display: inline-block;
  width: 60px;
  height: 34px;
}

.switch input {
  opacity: 0;
  width: 0;
  height: 0;
}

.slider {
  position: absolute;
  cursor: pointer;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  background-color: rgb(255, 255, 255);
  transition: 0.4s;
  border-radius: 34px;
}

.slider:before {
  position: absolute;
  content: "";
  height: 26px;
  width: 26px;
  left: 4px;
  bottom: 4px;
  background-color: rgb(128, 128, 128);
  transition: 0.4s;
  border-radius: 50%;
}

input:checked + .slider {
  background-color: rgb(0, 0, 0);
}

input:checked + .slider:before {
  transform: translateX(26px);
}

/* Optional for round sliders */
.slider.round {
  border-radius: 34px;
}

.slider.round:before {
  border-radius: 50%;
}
)rawliteral";