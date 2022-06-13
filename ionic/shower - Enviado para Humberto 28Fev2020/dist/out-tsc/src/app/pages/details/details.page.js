import * as tslib_1 from "tslib";
import { Component } from '@angular/core';
import { ProductService } from 'src/app/services/product.service';
import { ActivatedRoute } from '@angular/router';
import { NavController, LoadingController, ToastController } from '@ionic/angular';
import { AuthService } from 'src/app/services/auth.service';
let DetailsPage = class DetailsPage {
    constructor(productService, activatedRoute, navCtrl, loadingCtrl, authService, toastCtrl) {
        this.productService = productService;
        this.activatedRoute = activatedRoute;
        this.navCtrl = navCtrl;
        this.loadingCtrl = loadingCtrl;
        this.authService = authService;
        this.toastCtrl = toastCtrl;
        this.productId = null;
        this.product = {};
        this.productId = this.activatedRoute.snapshot.params['id'];
        if (this.productId)
            this.loadProduct();
    }
    ngOnInit() { }
    ngOnDestroy() {
        if (this.productSubscription)
            this.productSubscription.unsubscribe();
    }
    loadProduct() {
        this.productSubscription = this.productService.getProduct(this.productId).subscribe(data => {
            this.product = data;
        });
    }
    saveProduct() {
        return tslib_1.__awaiter(this, void 0, void 0, function* () {
            yield this.presentLoading();
            this.product.userId = this.authService.getAuth().currentUser.uid;
            if (this.productId) {
                try {
                    yield this.productService.updateProduct(this.productId, this.product);
                    yield this.loading.dismiss();
                    this.navCtrl.navigateBack('/home');
                }
                catch (error) {
                    console.error(error);
                    this.presentToast('Erro ao tentar salvar');
                    this.loading.dismiss();
                }
            }
            else {
                this.product.createdAt = new Date().getTime();
                try {
                    yield this.productService.addProduct(this.product);
                    yield this.loading.dismiss();
                    this.navCtrl.navigateBack('/home');
                }
                catch (error) {
                    console.error(error);
                    this.presentToast('Erro ao tentar salvar');
                    this.loading.dismiss();
                }
            }
        });
    }
    presentLoading() {
        return tslib_1.__awaiter(this, void 0, void 0, function* () {
            this.loading = yield this.loadingCtrl.create({ message: 'Aguarde...' });
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
DetailsPage = tslib_1.__decorate([
    Component({
        selector: 'app-details',
        templateUrl: './details.page.html',
        styleUrls: ['./details.page.scss'],
    }),
    tslib_1.__metadata("design:paramtypes", [ProductService,
        ActivatedRoute,
        NavController,
        LoadingController,
        AuthService,
        ToastController])
], DetailsPage);
export { DetailsPage };
//# sourceMappingURL=details.page.js.map