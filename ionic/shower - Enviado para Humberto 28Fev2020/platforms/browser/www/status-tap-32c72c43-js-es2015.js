(window["webpackJsonp"] = window["webpackJsonp"] || []).push([["status-tap-32c72c43-js"],{

/***/ "./node_modules/@ionic/core/dist/esm/status-tap-32c72c43.js":
/*!******************************************************************!*\
  !*** ./node_modules/@ionic/core/dist/esm/status-tap-32c72c43.js ***!
  \******************************************************************/
/*! exports provided: startStatusTap */
/***/ (function(module, __webpack_exports__, __webpack_require__) {

"use strict";
__webpack_require__.r(__webpack_exports__);
/* harmony export (binding) */ __webpack_require__.d(__webpack_exports__, "startStatusTap", function() { return startStatusTap; });
/* harmony import */ var _core_feeeff0d_js__WEBPACK_IMPORTED_MODULE_0__ = __webpack_require__(/*! ./core-feeeff0d.js */ "./node_modules/@ionic/core/dist/esm/core-feeeff0d.js");
/* harmony import */ var _config_3c7f3790_js__WEBPACK_IMPORTED_MODULE_1__ = __webpack_require__(/*! ./config-3c7f3790.js */ "./node_modules/@ionic/core/dist/esm/config-3c7f3790.js");



const startStatusTap = () => {
    const win = window;
    win.addEventListener('statusTap', () => {
        Object(_core_feeeff0d_js__WEBPACK_IMPORTED_MODULE_0__["f"])(() => {
            const width = win.innerWidth;
            const height = win.innerHeight;
            const el = document.elementFromPoint(width / 2, height / 2);
            if (!el) {
                return;
            }
            const contentEl = el.closest('ion-content');
            if (contentEl) {
                contentEl.componentOnReady().then(() => {
                    Object(_core_feeeff0d_js__WEBPACK_IMPORTED_MODULE_0__["w"])(() => contentEl.scrollToTop(300));
                });
            }
        });
    });
};




/***/ })

}]);