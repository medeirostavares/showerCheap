import * as tslib_1 from "tslib";
import { Component } from '@angular/core';
import { AuthService } from 'src/app/services/auth.service';
import { LoadingController, ToastController } from '@ionic/angular';
import { ProductService } from 'src/app/services/product.service';
let HomePage = class HomePage {
    constructor(authService, loadingCtrl, productService, toastCtrl) {
        this.authService = authService;
        this.loadingCtrl = loadingCtrl;
        this.productService = productService;
        this.toastCtrl = toastCtrl;
        this.products = new Array();
        this.productsSubscription = this.productService.getProducts().subscribe(data => {
            this.products = data;
        });
    }
    ngOnInit() { }
    ngOnDestroy() {
        this.productsSubscription.unsubscribe();
    }
    logout() {
        return tslib_1.__awaiter(this, void 0, void 0, function* () {
            yield this.presentLoading();
            try {
                yield this.authService.logout();
            }
            catch (error) {
                console.error(error);
            }
            finally {
                this.loading.dismiss();
            }
        });
    }
    presentLoading() {
        return tslib_1.__awaiter(this, void 0, void 0, function* () {
            this.loading = yield this.loadingCtrl.create({ message: 'Aguarde...' });
            return this.loading.present();
        });
    }
    deleteProduct(id) {
        return tslib_1.__awaiter(this, void 0, void 0, function* () {
            try {
                yield this.productService.deleteProduct(id);
            }
            catch (error) {
                this.presentToast('Erro ao tentar deletar');
            }
        });
    }
    presentToast(message) {
        return tslib_1.__awaiter(this, void 0, void 0, function* () {
            const toast = yield this.toastCtrl.create({ message, duration: 2000 });
            toast.present();
        });
    }
};
HomePage = tslib_1.__decorate([
    Component({
        selector: 'app-home',
        templateUrl: './home.page.html',
        styleUrls: ['./home.page.scss'],
    }),
    tslib_1.__metadata("design:paramtypes", [AuthService,
        LoadingController,
        ProductService,
        ToastController])
], HomePage);
export { HomePage };
//# sourceMappingURL=home.page.js.map