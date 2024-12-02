#pragma once
#include <Arduino.h>

const char* styles_css = R"rawliteral(
:root {
  --font-family: "Roboto", sans-serif;
  --font-size-base: 16px;
  --font-weight-regular: 400;
  --font-weight-semibold: 500;
  --font-weight-bold: 800;
}

* {
  padding: 0px;
  margin: 0px;
  box-sizing: border-box;
}

html,
body {
  min-height: 100svh;
  max-width: 100svw;
  height: fit-content;
  background-color: rgb(150, 150, 150);
  scrollbar-width: none;

  font-family: var(--font-family);
}

html::-webkit-scrollbar,
body::-webkit-scrollbar {
  display: none;
}

.heatingOn {
  background-color: lightcoral !important;
}

.heatingOff {
  background-color: cornflowerblue !important;
}

body {
  display: flex;
  flex-direction: column;
}

header {
  max-width: 100svw;
  display: flex;
  align-items: center;
  padding: clamp(0.1rem, 0.1rem + 1vw, 0.5rem);
  height: fit-content;
}

#homeLink {
  display: flex;
  color: white;
  text-decoration: none;
  align-items: center;
  user-select: none;
}

#headerLogo {
  height: clamp(1rem, 1rem + 5vw, 5rem);
  width: clamp(1rem, 1rem + 5vw, 5rem);
}

#headerHeading {
  margin-left: clamp(0.2rem, 0.2rem + 2vw, 1rem);
  font-size: clamp(1rem, 0.8rem + 5vw, 5rem);
  color: white;
  text-decoration: none;
}

.headerSpaser {
  flex-grow: 1;
}

#mainContent {
  display: flex;
  max-width: 100svw;
  flex: 1;
}

nav {
  display: flex;
  flex-direction: column;
  width: clamp(2rem, 5rem + 10svw, 20rem);
  margin: clamp(0.05rem, 0.05rem + 0.5svw, 0.25rem);
  margin-top: 0;
  margin-bottom: 0;
  padding: clamp(0.1rem, 0.1rem + 1svw, 0.5rem);
  padding-top: 0;
  padding-bottom: 0;
  border: 0.15vw solid rgba(255, 255, 255, 0.5);
  border-bottom: 0;
  border-radius: 1svw 1svw 0 0;
}

nav ul {
  list-style-type: none;
  display: flex;
  flex-direction: column;
  flex-grow: 1;
}

.navEl {
  border-radius: clamp(0.1rem, 0.2rem + 0.5svw, 1rem);
  margin-bottom: clamp(0.05rem, 0.05rem + 0.5svw, 0.25rem);
  transition: 0.25s;
}

.navEl p {
  display: block;
  text-align: center;
  padding: clamp(0.1rem, 0.1rem + 1svw, 0.5rem);
  padding-top: 0;
  padding-bottom: 0;
  font-size: clamp(0.7rem, 0.7rem + 1.5svw, 5rem);
  color: white;
}

.navEl a {
  display: block;
  padding: clamp(0.1rem, 0.1rem + 1svw, 0.5rem);
  font-size: clamp(0.5rem, 0.5rem + 1svw, 3rem);
  color: white;
  text-decoration: none;
}

.navEl#active {
  backdrop-filter: brightness(0.5);
}

.navEl:hover:not(#active, #settings) {
  backdrop-filter: brightness(1.5);
}

.navEl.last {
  margin-top: auto;
}

#slideMenuHolder {
  position: fixed;
  top: 0;
  left: -100svw;
  width: 100svw;
  height: 100svh;
  transition: 1s ease;
}

#slideMenuHolder.active {
  left: 0;
}

#slideMenu {
  height: 100%;
  width: fit-content;
  border: 2px solid black;
  border-left: 0;
  border-radius: 0 5.4svw 5.4svw 0;
  padding: 0.4rem;
  padding-left: 0;
  background-color: rgb(150, 150, 150);
}

.slideMenuNav {
  height: 100%;
  display: flex;
  flex-direction: column;
  width: fit-content;
  border: 0.15vw solid rgba(255, 255, 255, 0.5);
  border-left: 0;
  border-radius: 0 5svw 5svw 0;
}

.slideMenuNav ul {
  padding: clamp(0.1rem, 0.1rem + 1svw, 0.5rem);
  padding-top: 0;
  padding-bottom: 0;
  list-style-type: none;
  display: flex;
  flex-direction: column;
  flex-grow: 1;
}

.slideMenuNav li {
  width: 100%;
}

.slideNavEl {
  border-radius: clamp(0.1rem, 0.2rem + 0.5svw, 1rem);
  margin-bottom: clamp(0.05rem, 0.05rem + 0.5svw, 0.25rem);
  transition: 0.25s;
  width: fit-content;
}

.slideNavEl p {
  display: block;
  text-align: center;
  padding: 0.5rem 1rem;
  font-size: 2.5rem;
  color: white;
}

.slideNavEl a {
  display: block;
  padding: clamp(0.1rem, 0.1rem + 1svw, 0.5rem);
  font-size: 1.75rem;
  color: white;
  text-decoration: none;
}

.slideNavEl#active {
  backdrop-filter: brightness(0.5);
}

.slideNavEl:hover:not(#active, #settings) {
  backdrop-filter: brightness(1.5);
}

.slideNavEl.last {
  margin-top: auto;
}

#tab {
  max-width: 100svw;
  flex: 1;
  border: 0.1svw solid black;
  border-bottom: 0;
  border-radius: 2svw 2svw 0 0;
  background-color: white;
  padding: clamp(0.1rem, 0.1rem + 1svw, 0.5rem);
}

#tab.light-theme {
  background-color: rgb(255, 255, 255);
}

#tab.dark-theme {
  background-color: rgb(115, 115, 115);
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
  filter: opacity(0.2) grayscale(1);
}

body.heatingOn #heatingStatus {
  filter: opacity(1);
}

body.heatingOff #heatingStatus {
  filter: opacity(0.375);
}

body.heatingError #heatingStatus {
  filter: opacity(0.375) grayscale(1);
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

/* Heating mode switch */
.toggle-container {
  width: fit-content;
  font-weight: 900;
  font-size: 2rem;
  display: flex;
  margin: 1rem;
  margin-left: auto;
  margin-right: auto;
}

.toggle {
  width: 2rem;
  height: 5rem;
  background-color: cornflowerblue;
  border-radius: 1rem;
  padding: 0.2rem 0;
  cursor: pointer;
  display: flex;
  justify-content: center;
  transition: background-color 300ms 300ms;
}

.toggle__circle {
  width: 1.6rem;
  height: 1.6rem;
  background-color: hsl(0, 0%, 95%);
  border-radius: 50%;
  margin-top: calc(5rem - (0.2rem * 2) - 1.6rem);
  transition: margin 500ms ease-in-out;
}

.toggle-text {
  padding: 0.5rem;
  display: flex;
  justify-content: space-between;
  flex-direction: column;
  line-height: 0.8;
}

#check:checked + .toggle > .toggle__circle {
  margin-top: 0;
}

#check:checked + .toggle {
  background-color: lightcoral;
}

.container h1 {
  font-size: clamp(1rem, 0.8rem + 2vw, 5rem);
  text-align: center;
}

.light-theme .container h1 {
  color: #333;
}
.dark-theme .container h1 {
  color: white;
}

body #heatingGroup h1 {
  display: none;
}

body.heatingError #heatingGroup h1 {
  display: block;
  padding: 1rem;
  background-color: red;
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
  max-width: 100vw;
  padding: clamp(0.1rem, 0.1rem + 1vw, 0.5rem);
  height: fit-content;
  background-color: black;
  display: flex;
  justify-content: space-between;
}

footer p {
  font-size: clamp(0.5rem, 0.5rem + 1vw, 3rem);
  color: white;
}

footer div {
  display: flex;
}

footer div p {
  margin-right: clamp(0.1rem, 0.1rem + 1vw, 0.5rem);
}

#actionPendingCover.disable {
  display: none;
}

#actionPendingCover.enable {
  position: absolute;
  min-width: 100vw;
  height: 100vh;
  display: block;
}

#actionPendingCover:hover {
  cursor: wait;
}

.hoverWindow {
  min-width: 100vw;
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

#tab .loaderHolder {
  display: grid;
  place-items: center;
  height: 100%;
}

#tab .loaderHolder.disabled {
  display: none;
}

.loader {
  width: 10em;
  aspect-ratio: 1.154;
  --_g: no-repeat radial-gradient(farthest-side, rgba(0, 0, 0, 0.5) 90%, #0000);
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

  width: clamp(45px, 10vw, 90px);
  height: clamp(25.5px, 5.67vw, 51px);
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

  border-radius: (25.5px, 5.67vw, 51px);

  background-size: clamp(14.7px, 3.3vw, 29.25px);
  background-repeat: no-repeat;
  background-position: left clamp(5.25px, 1.1725vw, 10.5px) center,
    right clamp(5.25px, 1.1725vw, 10.5px) center;
  background-image: url("data:image/svg+xml,<svg xmlns='http://www.w3.org/2000/svg' width='15' height='15' viewBox='0 0 15 15'><style>.a%7Bfill:%23F0C419;%7D</style><g fill='none'><g class='a'><path d='M7.5 0C7.4 0 7.3 0.1 7.3 0.3L7.3 1.8C7.3 1.9 7.4 2 7.5 2 7.6 2 7.8 1.9 7.8 1.8L7.8 0.3C7.8 0.1 7.6 0 7.5 0Z'/><path d='M7.5 13C7.4 13 7.3 13.1 7.3 13.3L7.3 14.8C7.3 14.9 7.4 15 7.5 15 7.6 15 7.8 14.9 7.8 14.8L7.8 13.3C7.8 13.1 7.6 13 7.5 13Z'/><path d='M14.8 7.3L13.3 7.3C13.1 7.3 13 7.4 13 7.5 13 7.6 13.1 7.8 13.3 7.8L14.8 7.8C14.9 7.8 15 7.6 15 7.5 15 7.4 14.9 7.3 14.8 7.3Z'/><path d='M2 7.5C2 7.4 1.9 7.3 1.8 7.3L0.3 7.3C0.1 7.3 0 7.4 0 7.5 0 7.6 0.1 7.8 0.3 7.8L1.8 7.8C1.9 7.8 2 7.6 2 7.5Z'/><path d='M11.6 3.7C11.6 3.7 11.7 3.7 11.7 3.6L13.2 2.2C13.3 2.1 13.3 1.9 13.2 1.8 13.1 1.7 12.9 1.7 12.8 1.8L11.4 3.3C11.3 3.4 11.3 3.5 11.4 3.6 11.4 3.7 11.5 3.7 11.6 3.7Z'/><path d='M3.3 11.4L1.8 12.8C1.7 12.9 1.7 13.1 1.8 13.2 1.9 13.2 1.9 13.3 2 13.3 2.1 13.3 2.1 13.2 2.2 13.2L3.6 11.7C3.7 11.6 3.7 11.5 3.6 11.4 3.5 11.3 3.4 11.3 3.3 11.4Z'/><path d='M11.7 11.4C11.6 11.3 11.5 11.3 11.4 11.4 11.3 11.5 11.3 11.6 11.4 11.7L12.8 13.2C12.9 13.2 12.9 13.3 13 13.3 13.1 13.3 13.1 13.2 13.2 13.2 13.3 13.1 13.3 12.9 13.2 12.8L11.7 11.4Z'/><path d='M2.2 1.8C2.1 1.7 1.9 1.7 1.8 1.8 1.7 1.9 1.7 2.1 1.8 2.2L3.3 3.6C3.3 3.7 3.4 3.7 3.4 3.7 3.5 3.7 3.6 3.7 3.6 3.6 3.7 3.5 3.7 3.4 3.6 3.3L2.2 1.8Z'/><path d='M12.6 5.4C12.6 5.4 12.7 5.5 12.8 5.5 12.8 5.5 12.9 5.5 12.9 5.5L13.6 5.2C13.7 5.1 13.8 5 13.7 4.9 13.7 4.7 13.5 4.7 13.4 4.7L12.7 5C12.6 5.1 12.5 5.2 12.6 5.4Z'/><path d='M2.4 9.6C2.4 9.5 2.2 9.5 2.1 9.5L1.4 9.8C1.3 9.9 1.2 10 1.3 10.1 1.3 10.2 1.4 10.3 1.5 10.3 1.5 10.3 1.6 10.3 1.6 10.3L2.3 10C2.4 9.9 2.5 9.8 2.4 9.6Z'/><path d='M13.6 9.7L12.9 9.4C12.8 9.4 12.7 9.4 12.6 9.6 12.5 9.7 12.6 9.8 12.7 9.9L13.4 10.2C13.5 10.2 13.5 10.2 13.5 10.2 13.6 10.2 13.7 10.1 13.8 10 13.8 9.9 13.7 9.8 13.6 9.7Z'/><path d='M1.4 5.3L2.1 5.6C2.1 5.6 2.1 5.6 2.2 5.6 2.3 5.6 2.4 5.5 2.4 5.4 2.5 5.3 2.4 5.2 2.3 5.1L1.6 4.8C1.4 4.8 1.3 4.8 1.2 5 1.2 5.1 1.3 5.2 1.4 5.3Z'/><path d='M10 12.7C9.9 12.6 9.8 12.5 9.6 12.6 9.5 12.6 9.5 12.8 9.5 12.9L9.8 13.6C9.8 13.7 9.9 13.7 10 13.7 10.1 13.7 10.1 13.7 10.1 13.7 10.3 13.7 10.3 13.5 10.3 13.4L10 12.7Z'/><path d='M5.3 2.5C5.3 2.5 5.3 2.5 5.4 2.4 5.5 2.4 5.5 2.2 5.5 2.1L5.2 1.4C5.1 1.3 5 1.2 4.9 1.3 4.7 1.3 4.7 1.5 4.7 1.6L5 2.3C5.1 2.4 5.2 2.5 5.3 2.5Z'/><path d='M5.4 12.6C5.3 12.5 5.2 12.6 5.1 12.7L4.8 13.4C4.8 13.6 4.8 13.7 5 13.8 5 13.8 5 13.8 5.1 13.8 5.2 13.8 5.3 13.7 5.3 13.6L5.6 12.9C5.6 12.8 5.6 12.7 5.4 12.6Z'/><path d='M9.7 2.4C9.8 2.4 9.8 2.4 9.9 2.3L10.2 1.6C10.2 1.4 10.2 1.3 10 1.2 9.9 1.2 9.8 1.3 9.7 1.4L9.4 2.1C9.4 2.2 9.4 2.3 9.6 2.4 9.6 2.4 9.6 2.4 9.7 2.4Z'/></g><circle cx='7.5' cy='7.5' r='5' class='a'/><circle cx='7.5' cy='7.5' r='3.8' fill='%23EDE21B'/></g></svg>%0A"),
    url("data:image/svg+xml,<%3Fxml version='1.0' encoding='UTF-8'%3F><svg width='15px' height='15px' viewBox='0 0 15 15' version='1.1' xmlns='http://www.w3.org/2000/svg' xmlns:xlink='http://www.w3.org/1999/xlink'><g id='Page-1' stroke='none' stroke-width='1' fill='none' fill-rule='evenodd'><g id='Film' transform='translate(-1566.000000, -1086.000000)' fill-rule='nonzero'><g id='Group' transform='translate(1566.000000, 1086.000000)'><path d='M4.4064,11.26584 C8.19456,11.26584 11.26512,8.1948 11.26512,4.40664 C11.26512,2.72808 10.66032,1.19256 9.65952,0.00024 C12.75168,0.99288 14.99088,3.8892 14.99088,7.31112 C14.99088,11.55288 11.55264,14.99112 7.31088,14.99112 C3.88896,14.99112 0.99264,12.75192 8.52651283e-16,9.65976 C1.19232,10.66104 2.72832,11.26584 4.4064,11.26584 Z' id='Path' fill='%23FFD93B'></path><path d='M12.0576,1.28328 C13.0848,2.5884 13.70208,4.23144 13.70208,6.02184 C13.70208,10.2636 10.26384,13.70184 6.02208,13.70184 C4.23168,13.70184 2.58864,13.08456 1.28352,12.05736 C2.68896,13.84152 4.86384,14.99112 7.31136,14.99112 C11.55312,14.99112 14.99136,11.55288 14.99136,7.31112 C14.99136,4.8636 13.84176,2.6892 12.0576,1.28328 Z' id='Path' fill='%23F4C534'></path><g transform='translate(1.260000, 2.460000)' fill='%23FFD83B' id='Path'><polygon points='2.58384 0.53832 3.37488 2.142 5.14416 2.3988 3.864 3.6468 4.16592 5.40888 2.58384 4.57704 1.00128 5.40888 1.3032 3.6468 0.02304 2.3988 1.79232 2.142'></polygon><polygon points='7.04592 0.0252 7.45584 0.85656 8.37312 0.98952 7.70928 1.63656 7.86624 2.54952 7.04592 2.11848 6.2256 2.54952 6.38256 1.63656 5.71872 0.98952 6.636 0.85656'></polygon></g></g></g></g></svg>");
}

.slider:before {
  position: absolute;
  content: "";

  height: clamp(19.5px, 4.33vw, 39px);
  width: clamp(19.5px, 4.33vw, 39px);

  left: clamp(3px, 0.67vw, 6px);
  bottom: clamp(3px, 0.67vw, 6px);

  background-color: rgba(128, 128, 128, 1);
  transition: 0.4s;
  border-radius: 50%;
}

input:checked + .slider {
  background-color: rgb(0, 0, 0);
}

input:checked + .slider:before {
  transform: translateX(clamp(19.5px, 4.33vw, 39px));
}

/* Optional for round sliders */
.slider.round {
  border-radius: clamp(25.5px, 5.67vw, 51px);
}

.slider.round:before {
  border-radius: 50%;
}

.container {
  background-color: transparent;
  padding: 0;
  border-radius: 0;
}

.container.disabled {
  display: none;
}

.timer-item {
  margin-bottom: 1vh;
  padding: 1vw;
  border-radius: 1rem;
  max-width: 100%;
  box-sizing: border-box;
}

#tab.light-theme .timer-item,
#tab.light-theme .timer-item .time-range {
  border: 1px solid rgba(0, 0, 0, 0.5);
  background: rgba(0, 0, 0, 0.25);
}
#tab.dark-theme .timer-item,
#tab.dark-theme .timer-item .time-range {
  border: 1px solid rgb(255, 255, 255, 0.5);
  background: rgb(255, 255, 255, 0.25);
}

.timer-item select,
.timer-item input[type="time"] {
  padding: 0.5rem;
  margin: 0.5rem;
  width: fit-content;
}

.timer-item-header label {
  text-align: center;
}

.container button {
  padding: 0.75rem 1.25rem;
  margin: 1rem;
  border: none;
  border-radius: 4px;
  cursor: pointer;
}

.container button:hover {
  filter: brightness(0.85);
}

.container button#delete {
  background-color: lightcoral;
}

.container button.disabled {
  color: transparent;
  background-color: transparent;
  border: none;
  pointer-events: none;

  display: none;
}

label {
  font-weight: bold;
}

.timer-item-header,
.time-range {
  display: flex;
  align-items: center;
  justify-content: space-between;
  flex-wrap: wrap;
  gap: 5rem;
}

.timer-item-header button {
  margin-top: 0 !important;
  margin-bottom: 0 !important;
}

.time-range {
  display: flex;
  align-items: center;
  margin-top: 0.5rem;
  padding: 0 1rem;
  border-radius: 0.5rem;
}

.timePeriod {
  display: flex;
  align-items: center;
  justify-content: space-between;
  flex-direction: row !important;
  width: max-content;
  flex: 1;
}

.time-range button {
  margin: 0;
}

.timer-item-header div {
  display: flex;
  flex-direction: column;
}

.timer-buttons {
  display: flex;
  justify-content: flex-end;
  margin-top: 0.5rem;
}

/* ---------------------------------------- */

#about-section {
  padding: 2rem;
  border-radius: 10px;
  margin: 2rem auto;
  max-width: 850px;
  text-align: left;
  font-family: var(--font-family);
}

#about-section h2 {
  font-weight: bold;
  margin-bottom: 1rem;
  color: black;
  text-align: center;
}

#about-section h3 {
  font-weight: bold;
  margin-top: 1.5rem;
  margin-bottom: 1rem;
  color: black;
}

#about-section p {
  line-height: 1.8;
  color: black;
  margin-bottom: 1rem;
}

#about-section ul {
  list-style-type: disc;
  padding-left: 2rem;
  margin-bottom: 1.5rem;
}

#about-section ul li {
  line-height: 1.6;
  margin-bottom: 0.75rem;
  color: black;
}

#about-section a {
  color: #0059b9;
  text-decoration: none;
  font-weight: bold;
}

#about-section a:hover {
  text-decoration: underline;
}

#fun-fact {
  margin-top: 1.5rem;
  padding: 1rem;
  background-color: rgba(255, 243, 205, 0.6);
  border-left: 5px solid #ffc107;
  border-radius: 5px;
  font-style: italic;
  color: #aa8410;
}

/* ---------------------------------------- */

@media (max-width: 700px) {
  .container {
    padding: 0.5rem;
  }

  .container h1 {
    font-size: 1.5rem;
  }

  .timer-item {
    margin-bottom: 0.5rem;
    padding: 0.5rem;
  }

  .timer-item-header,
  .time-range {
    flex-direction: column;
    align-items: center;
    gap: 0.5rem;
  }

  .timePeriod {
    width: 100%;
    justify-content: space-evenly;
  }

  .time-range {
    padding: 0.5rem;
  }

  .timeContainer {
    display: flex;
    flex-direction: column;
    align-items: center;
  }

  .timer-item button {
    padding: 0.6rem 0.75rem;
    margin: 0.25rem;
  }
}

@media (max-width: 500px) {
  nav {
    display: none;
  }

  .logoBtn {
    display: block;
  }
}

@media (max-width: 320px) {
  .toggle-text {
    font-size: 1.5rem;
  }
}

)rawliteral";
