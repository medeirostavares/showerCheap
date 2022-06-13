import * as tslib_1 from "tslib";
import { Component, ViewChild } from '@angular/core';
import { IonSlides, LoadingController, ToastController } from '@ionic/angular';
import { Keyboard } from '@ionic-native/keyboard/ngx';
import { AuthService } from 'src/app/services/auth.service';
let LoginPage = class LoginPage {
    constructor(keyboard, loadingCtrl, toastCtrl, authService) {
        this.keyboard = keyboard;
        this.loadingCtrl = loadingCtrl;
        this.toastCtrl = toastCtrl;
        this.authService = authService;
        this.wavesPosition = 0;
        this.wavesDifference = 80;
        this.userLogin = {};
        this.userRegister = {};
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
    login() {
        return tslib_1.__awaiter(this, void 0, void 0, function* () {
            yield this.presentLoading();
            try {
                yield this.authService.login(this.userLogin);
            }
            catch (error) {
                console.error(error);
                this.presentToast(error.message);
            }
            finally {
                this.loading.dismiss();
            }
        });
    }
    register() {
        return tslib_1.__awaiter(this, void 0, void 0, function* () {
            yield this.presentLoading();
            try {
                yield this.authService.register(this.userRegister);
            }
            catch (error) {
                console.error(error);
                this.presentToast(error.message);
            }
            finally {
                this.loading.dismiss();
            }
        });
    }
    presentLoading() {
        return tslib_1.__awaiter(this, void 0, void 0, function* () {
            this.loading = yield this.loadingCtrl.create({
                message: 'Aguarde...',
            });
            return this.loading.present();
        });
    }
    presentToast(message) {
        return tslib_1.__awaiter(this, void 0, void 0, function* () {
            const toast = yield this.toastCtrl.create({ message, duration: 2000 });
            toast.present();
        });
    }
};
tslib_1.__decorate([
    ViewChild(IonSlides, { static: false }),
    tslib_1.__metadata("design:type", IonSlides)
], LoginPage.prototype, "slides", void 0);
LoginPage = tslib_1.__decorate([
    Component({
        selector: 'app-login',
        templateUrl: './login.page.html',
        styleUrls: ['./login.page.scss'],
    }),
    tslib_1.__metadata("design:paramtypes", [Keyboard,
        LoadingController,
        ToastController,
        AuthService])
], LoginPage);
export { LoginPage };
//# sourceMappingURL=login.page.js.map