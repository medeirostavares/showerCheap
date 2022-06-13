(window["webpackJsonp"] = window["webpackJsonp"] || []).push([["pages-login-login-module"],{

/***/ "./node_modules/raw-loader/dist/cjs.js!./src/app/pages/login/login.page.html":
/*!***********************************************************************************!*\
  !*** ./node_modules/raw-loader/dist/cjs.js!./src/app/pages/login/login.page.html ***!
  \***********************************************************************************/
/*! exports provided: default */
/***/ (function(module, __webpack_exports__, __webpack_require__) {

"use strict";
__webpack_require__.r(__webpack_exports__);
/* harmony default export */ __webpack_exports__["default"] = ("<ion-content class=\"swiper-no-swiping\">\n  <div class=\"container top\">\n    <img src=\"/assets/img/logo.png\">\n    <ion-segment (ionChange)=\"segmentChanged($event)\" value=\"login\">\n        <ion-segment-button value=\"login\">\n          <ion-label>Login</ion-label>\n        </ion-segment-button>\n        <ion-segment-button value=\"register\">\n          <ion-label>Cadastro</ion-label>\n        </ion-segment-button>\n      </ion-segment>\n  </div>\n    <ion-slides>\n        <ion-slide>\n          <div class=\"slide-inner ion-padding\">\n            <span>Login</span>\n    \n            <ion-item class=\"ion-margin-top\" lines=\"none\">\n              <ion-input type=\"email\" placeholder=\"E-mail\"></ion-input>\n            </ion-item>\n    \n            <ion-item class=\"ion-margin-top\" lines=\"none\">\n              <ion-input type=\"password\" placeholder=\"Senha\"></ion-input>\n            </ion-item>\n    \n            <ion-button class=\"ion-margin-top\" (click)=\"login()\" color=\"light\" expand=\"block\">\n              Entrar\n            </ion-button>\n          </div>\n        </ion-slide>\n    \n        <ion-slide>\n          <div class=\"slide-inner ion-padding\">\n            <span>Cadastro</span>\n    \n            <ion-item class=\"ion-margin-top\" lines=\"none\">\n              <ion-input type=\"text\" placeholder=\"Nome\"></ion-input>\n            </ion-item>\n    \n            <ion-item class=\"ion-margin-top\" lines=\"none\">\n              <ion-input type=\"email\" placeholder=\"E-mail\"></ion-input>\n            </ion-item>\n    \n            <ion-item class=\"ion-margin-top\" lines=\"none\">\n              <ion-input type=\"password\" placeholder=\"Senha\"></ion-input>\n            </ion-item>\n    \n            <ion-button class=\"ion-margin-top\" (click)=\"register()\" color=\"light\" expand=\"block\">\n              Cadastrar\n            </ion-button>\n          </div>\n        </ion-slide>\n      </ion-slides>\n\n  <div class=\"ocean\">\n    <div class=\"waves\" [ngStyle]=\"{'background-position-x': wavesPosition + 'px'}\"></div>\n\n    <div class=\"container bottom ion-text-center\">\n      <span>Seu banho muito mais econômico!</span>\n    </div>\n\n  </div>\n\n</ion-content>\n");

/***/ }),

/***/ "./src/app/pages/login/login-routing.module.ts":
/*!*****************************************************!*\
  !*** ./src/app/pages/login/login-routing.module.ts ***!
  \*****************************************************/
/*! exports provided: LoginPageRoutingModule */
/***/ (function(module, __webpack_exports__, __webpack_require__) {

"use strict";
__webpack_require__.r(__webpack_exports__);
/* harmony export (binding) */ __webpack_require__.d(__webpack_exports__, "LoginPageRoutingModule", function() { return LoginPageRoutingModule; });
/* harmony import */ var tslib__WEBPACK_IMPORTED_MODULE_0__ = __webpack_require__(/*! tslib */ "./node_modules/tslib/tslib.es6.js");
/* harmony import */ var _angular_core__WEBPACK_IMPORTED_MODULE_1__ = __webpack_require__(/*! @angular/core */ "./node_modules/@angular/core/fesm2015/core.js");
/* harmony import */ var _angular_router__WEBPACK_IMPORTED_MODULE_2__ = __webpack_require__(/*! @angular/router */ "./node_modules/@angular/router/fesm2015/router.js");
/* harmony import */ var _login_page__WEBPACK_IMPORTED_MODULE_3__ = __webpack_require__(/*! ./login.page */ "./src/app/pages/login/login.page.ts");




const routes = [
    {
        path: '',
        component: _login_page__WEBPACK_IMPORTED_MODULE_3__["LoginPage"]
    }
];
let LoginPageRoutingModule = class LoginPageRoutingModule {
};
LoginPageRoutingModule = tslib__WEBPACK_IMPORTED_MODULE_0__["__decorate"]([
    Object(_angular_core__WEBPACK_IMPORTED_MODULE_1__["NgModule"])({
        imports: [_angular_router__WEBPACK_IMPORTED_MODULE_2__["RouterModule"].forChild(routes)],
        exports: [_angular_router__WEBPACK_IMPORTED_MODULE_2__["RouterModule"]],
    })
], LoginPageRoutingModule);



/***/ }),

/***/ "./src/app/pages/login/login.module.ts":
/*!*********************************************!*\
  !*** ./src/app/pages/login/login.module.ts ***!
  \*********************************************/
/*! exports provided: LoginPageModule */
/***/ (function(module, __webpack_exports__, __webpack_require__) {

"use strict";
__webpack_require__.r(__webpack_exports__);
/* harmony export (binding) */ __webpack_require__.d(__webpack_exports__, "LoginPageModule", function() { return LoginPageModule; });
/* harmony import */ var tslib__WEBPACK_IMPORTED_MODULE_0__ = __webpack_require__(/*! tslib */ "./node_modules/tslib/tslib.es6.js");
/* harmony import */ var _angular_core__WEBPACK_IMPORTED_MODULE_1__ = __webpack_require__(/*! @angular/core */ "./node_modules/@angular/core/fesm2015/core.js");
/* harmony import */ var _angular_common__WEBPACK_IMPORTED_MODULE_2__ = __webpack_require__(/*! @angular/common */ "./node_modules/@angular/common/fesm2015/common.js");
/* harmony import */ var _angular_forms__WEBPACK_IMPORTED_MODULE_3__ = __webpack_require__(/*! @angular/forms */ "./node_modules/@angular/forms/fesm2015/forms.js");
/* harmony import */ var _ionic_angular__WEBPACK_IMPORTED_MODULE_4__ = __webpack_require__(/*! @ionic/angular */ "./node_modules/@ionic/angular/dist/fesm5.js");
/* harmony import */ var _login_routing_module__WEBPACK_IMPORTED_MODULE_5__ = __webpack_require__(/*! ./login-routing.module */ "./src/app/pages/login/login-routing.module.ts");
/* harmony import */ var _login_page__WEBPACK_IMPORTED_MODULE_6__ = __webpack_require__(/*! ./login.page */ "./src/app/pages/login/login.page.ts");







let LoginPageModule = class LoginPageModule {
};
LoginPageModule = tslib__WEBPACK_IMPORTED_MODULE_0__["__decorate"]([
    Object(_angular_core__WEBPACK_IMPORTED_MODULE_1__["NgModule"])({
        imports: [
            _angular_common__WEBPACK_IMPORTED_MODULE_2__["CommonModule"],
            _angular_forms__WEBPACK_IMPORTED_MODULE_3__["FormsModule"],
            _ionic_angular__WEBPACK_IMPORTED_MODULE_4__["IonicModule"],
            _login_routing_module__WEBPACK_IMPORTED_MODULE_5__["LoginPageRoutingModule"]
        ],
        declarations: [_login_page__WEBPACK_IMPORTED_MODULE_6__["LoginPage"]]
    })
], LoginPageModule);



/***/ }),

/***/ "./src/app/pages/login/login.page.scss":
/*!*********************************************!*\
  !*** ./src/app/pages/login/login.page.scss ***!
  \*********************************************/
/*! exports provided: default */
/***/ (function(module, __webpack_exports__, __webpack_require__) {

"use strict";
__webpack_require__.r(__webpack_exports__);
/* harmony default export */ __webpack_exports__["default"] = (".container {\n  position: absolute;\n  left: 0;\n  right: 0;\n  width: 75%;\n  margin: 0 auto;\n  z-index: 2;\n}\n.container.top {\n  top: 5%;\n}\n.container.bottom {\n  bottom: 30%;\n}\n.container.bottom span {\n  color: #fff;\n}\nion-slides {\n  height: 100%;\n}\nion-slides .slide-inner {\n  width: 75%;\n  color: #fff;\n  background-color: #0072bc;\n  border-radius: 10px;\n}\nion-slides .slide-inner ion-item {\n  --background: rgba(255, 255,255,0.2);\n  --border-radius: 10px;\n}\nion-slides .slide-inner ion-item ion-input {\n  color: #fff;\n}\n.ocean {\n  position: absolute;\n  left: 0;\n  right: 0;\n  bottom: 0;\n  height: 45%;\n  background-color: var(--ion-color-primary);\n}\n.ocean .waves {\n  position: absolute;\n  top: -100px;\n  left: 0;\n  right: 0;\n  height: 100px;\n  background-image: url(\"/assets/img/waves.png\");\n  background-size: contain;\n  transition: all 0.5s;\n}\n/*# sourceMappingURL=data:application/json;base64,eyJ2ZXJzaW9uIjozLCJzb3VyY2VzIjpbIi9Vc2Vycy9mYWJpYW5vL0RvY3VtZW50cy9Qcm9mZXNzb3IgLSBEb2NzL0dpdEh1YiAtIFByb2pldG9zL2lvbmljL3Nob3dlci9zcmMvYXBwL3BhZ2VzL2xvZ2luL2xvZ2luLnBhZ2Uuc2NzcyIsInNyYy9hcHAvcGFnZXMvbG9naW4vbG9naW4ucGFnZS5zY3NzIl0sIm5hbWVzIjpbXSwibWFwcGluZ3MiOiJBQUFBO0VBQ0ksa0JBQUE7RUFDQSxPQUFBO0VBQ0EsUUFBQTtFQUNBLFVBQUE7RUFDQSxjQUFBO0VBQ0EsVUFBQTtBQ0NKO0FEQUk7RUFDSSxPQUFBO0FDRVI7QURDSTtFQUNJLFdBQUE7QUNDUjtBRENRO0VBQ0ksV0FBQTtBQ0NaO0FESUE7RUFDRyxZQUFBO0FDREg7QURJSTtFQUNJLFVBQUE7RUFDQSxXQUFBO0VBQ0EseUJBQUE7RUFDQSxtQkFBQTtBQ0ZSO0FES1E7RUFDSSxvQ0FBQTtFQUNBLHFCQUFBO0FDSFo7QURLWTtFQUNJLFdBQUE7QUNIaEI7QURTQTtFQUNBLGtCQUFBO0VBQ0EsT0FBQTtFQUNBLFFBQUE7RUFDQSxTQUFBO0VBQ0EsV0FBQTtFQUNBLDBDQUFBO0FDTkE7QURRSTtFQUNJLGtCQUFBO0VBQ0EsV0FBQTtFQUNBLE9BQUE7RUFDQSxRQUFBO0VBQ0EsYUFBQTtFQUNBLDhDQUFBO0VBQ0Esd0JBQUE7RUFDQSxvQkFBQTtBQ05SIiwiZmlsZSI6InNyYy9hcHAvcGFnZXMvbG9naW4vbG9naW4ucGFnZS5zY3NzIiwic291cmNlc0NvbnRlbnQiOlsiLmNvbnRhaW5lciB7XG4gICAgcG9zaXRpb246IGFic29sdXRlO1xuICAgIGxlZnQ6IDA7XG4gICAgcmlnaHQ6IDA7XG4gICAgd2lkdGg6IDc1JTtcbiAgICBtYXJnaW46IDAgYXV0bztcbiAgICB6LWluZGV4OiAyO1xuICAgICYudG9wIHtcbiAgICAgICAgdG9wOiA1JTtcbiAgICB9XG5cbiAgICAmLmJvdHRvbSB7XG4gICAgICAgIGJvdHRvbTogMzAlO1xuXG4gICAgICAgIHNwYW4ge1xuICAgICAgICAgICAgY29sb3I6ICNmZmY7XG4gICAgICAgIH1cbiAgICB9XG59XG5cbmlvbi1zbGlkZXMge1xuICAgaGVpZ2h0OiAxMDAlOyBcblxuXG4gICAgLnNsaWRlLWlubmVyIHtcbiAgICAgICAgd2lkdGg6IDc1JTtcbiAgICAgICAgY29sb3I6ICNmZmY7XG4gICAgICAgIGJhY2tncm91bmQtY29sb3I6ICMwMDcyYmM7XG4gICAgICAgIGJvcmRlci1yYWRpdXM6IDEwcHg7XG4gICAgXG4gICAgXG4gICAgICAgIGlvbi1pdGVtIHtcbiAgICAgICAgICAgIC0tYmFja2dyb3VuZDogcmdiYSgyNTUsIDI1NSwyNTUsMC4yKTtcbiAgICAgICAgICAgIC0tYm9yZGVyLXJhZGl1czogMTBweDtcblxuICAgICAgICAgICAgaW9uLWlucHV0IHtcbiAgICAgICAgICAgICAgICBjb2xvcjogI2ZmZjtcbiAgICAgICAgICAgIH1cbiAgICAgICAgfVxuICAgIH1cbn1cblxuLm9jZWFuIHtcbnBvc2l0aW9uOiBhYnNvbHV0ZTtcbmxlZnQ6IDA7XG5yaWdodDogMDtcbmJvdHRvbTogMDtcbmhlaWdodDogNDUlO1xuYmFja2dyb3VuZC1jb2xvcjogdmFyKC0taW9uLWNvbG9yLXByaW1hcnkpO1xuXG4gICAgLndhdmVzIHtcbiAgICAgICAgcG9zaXRpb246IGFic29sdXRlO1xuICAgICAgICB0b3A6IC0xMDBweDtcbiAgICAgICAgbGVmdDogMDtcbiAgICAgICAgcmlnaHQ6IDA7XG4gICAgICAgIGhlaWdodDogMTAwcHg7XG4gICAgICAgIGJhY2tncm91bmQtaW1hZ2U6IHVybCgnL2Fzc2V0cy9pbWcvd2F2ZXMucG5nJyk7XG4gICAgICAgIGJhY2tncm91bmQtc2l6ZTogY29udGFpbjtcbiAgICAgICAgdHJhbnNpdGlvbjogYWxsIDAuNXM7XG4gICAgfVxufSIsIi5jb250YWluZXIge1xuICBwb3NpdGlvbjogYWJzb2x1dGU7XG4gIGxlZnQ6IDA7XG4gIHJpZ2h0OiAwO1xuICB3aWR0aDogNzUlO1xuICBtYXJnaW46IDAgYXV0bztcbiAgei1pbmRleDogMjtcbn1cbi5jb250YWluZXIudG9wIHtcbiAgdG9wOiA1JTtcbn1cbi5jb250YWluZXIuYm90dG9tIHtcbiAgYm90dG9tOiAzMCU7XG59XG4uY29udGFpbmVyLmJvdHRvbSBzcGFuIHtcbiAgY29sb3I6ICNmZmY7XG59XG5cbmlvbi1zbGlkZXMge1xuICBoZWlnaHQ6IDEwMCU7XG59XG5pb24tc2xpZGVzIC5zbGlkZS1pbm5lciB7XG4gIHdpZHRoOiA3NSU7XG4gIGNvbG9yOiAjZmZmO1xuICBiYWNrZ3JvdW5kLWNvbG9yOiAjMDA3MmJjO1xuICBib3JkZXItcmFkaXVzOiAxMHB4O1xufVxuaW9uLXNsaWRlcyAuc2xpZGUtaW5uZXIgaW9uLWl0ZW0ge1xuICAtLWJhY2tncm91bmQ6IHJnYmEoMjU1LCAyNTUsMjU1LDAuMik7XG4gIC0tYm9yZGVyLXJhZGl1czogMTBweDtcbn1cbmlvbi1zbGlkZXMgLnNsaWRlLWlubmVyIGlvbi1pdGVtIGlvbi1pbnB1dCB7XG4gIGNvbG9yOiAjZmZmO1xufVxuXG4ub2NlYW4ge1xuICBwb3NpdGlvbjogYWJzb2x1dGU7XG4gIGxlZnQ6IDA7XG4gIHJpZ2h0OiAwO1xuICBib3R0b206IDA7XG4gIGhlaWdodDogNDUlO1xuICBiYWNrZ3JvdW5kLWNvbG9yOiB2YXIoLS1pb24tY29sb3ItcHJpbWFyeSk7XG59XG4ub2NlYW4gLndhdmVzIHtcbiAgcG9zaXRpb246IGFic29sdXRlO1xuICB0b3A6IC0xMDBweDtcbiAgbGVmdDogMDtcbiAgcmlnaHQ6IDA7XG4gIGhlaWdodDogMTAwcHg7XG4gIGJhY2tncm91bmQtaW1hZ2U6IHVybChcIi9hc3NldHMvaW1nL3dhdmVzLnBuZ1wiKTtcbiAgYmFja2dyb3VuZC1zaXplOiBjb250YWluO1xuICB0cmFuc2l0aW9uOiBhbGwgMC41cztcbn0iXX0= */");

/***/ }),

/***/ "./src/app/pages/login/login.page.ts":
/*!*******************************************!*\
  !*** ./src/app/pages/login/login.page.ts ***!
  \*******************************************/
/*! exports provided: LoginPage */
/***/ (function(module, __webpack_exports__, __webpack_require__) {

"use strict";
__webpack_require__.r(__webpack_exports__);
/* harmony export (binding) */ __webpack_require__.d(__webpack_exports__, "LoginPage", function() { return LoginPage; });
/* harmony import */ var tslib__WEBPACK_IMPORTED_MODULE_0__ = __webpack_require__(/*! tslib */ "./node_modules/tslib/tslib.es6.js");
/* harmony import */ var _angular_core__WEBPACK_IMPORTED_MODULE_1__ = __webpack_require__(/*! @angular/core */ "./node_modules/@angular/core/fesm2015/core.js");
/* harmony import */ var _ionic_angular__WEBPACK_IMPORTED_MODULE_2__ = __webpack_require__(/*! @ionic/angular */ "./node_modules/@ionic/angular/dist/fesm5.js");



let LoginPage = class LoginPage {
    constructor() {
        this.wavesPosition = 0;
        this.wavesDifference = 80;
    }
    ngOnInit() { }
    segmentChanged(event) {
        if (event.detail.value === "login") {
            this.slides.slidePrev();
            this.wavesPosition += this.wavesDifference;
        }
        else {
            this.slides.slideNext();
            this.wavesPosition -= this.wavesDifference;
        }
    }
};
tslib__WEBPACK_IMPORTED_MODULE_0__["__decorate"]([
    Object(_angular_core__WEBPACK_IMPORTED_MODULE_1__["ViewChild"])(_ionic_angular__WEBPACK_IMPORTED_MODULE_2__["IonSlides"], { static: false }),
    tslib__WEBPACK_IMPORTED_MODULE_0__["__metadata"]("design:type", _ionic_angular__WEBPACK_IMPORTED_MODULE_2__["IonSlides"])
], LoginPage.prototype, "slides", void 0);
LoginPage = tslib__WEBPACK_IMPORTED_MODULE_0__["__decorate"]([
    Object(_angular_core__WEBPACK_IMPORTED_MODULE_1__["Component"])({
        selector: 'app-login',
        template: tslib__WEBPACK_IMPORTED_MODULE_0__["__importDefault"](__webpack_require__(/*! raw-loader!./login.page.html */ "./node_modules/raw-loader/dist/cjs.js!./src/app/pages/login/login.page.html")).default,
        styles: [tslib__WEBPACK_IMPORTED_MODULE_0__["__importDefault"](__webpack_require__(/*! ./login.page.scss */ "./src/app/pages/login/login.page.scss")).default]
    }),
    tslib__WEBPACK_IMPORTED_MODULE_0__["__metadata"]("design:paramtypes", [])
], LoginPage);



/***/ })

}]);