import * as tslib_1 from "tslib";
import { Injectable } from '@angular/core';
import { AngularFirestore } from '@angular/fire/firestore';
import { map } from 'rxjs/operators';
let ProductService = class ProductService {
    constructor(afs) {
        this.afs = afs;
        this.productsCollection = this.afs.collection('Products');
    }
    getProducts() {
        return this.productsCollection.snapshotChanges().pipe(map(actions => {
            return actions.map(a => {
                const data = a.payload.doc.data();
                const id = a.payload.doc.id;
                return Object.assign({ id }, data);
            });
        }));
    }
    addProduct(product) {
        return this.productsCollection.add(product);
    }
    getProduct(id) {
        return this.productsCollection.doc(id).valueChanges();
    }
    updateProduct(id, product) {
        return this.productsCollection.doc(id).update(product);
    }
    deleteProduct(id) {
        return this.productsCollection.doc(id).delete();
    }
};
ProductService = tslib_1.__decorate([
    Injectable({
        providedIn: 'root'
    }),
    tslib_1.__metadata("design:paramtypes", [AngularFirestore])
], ProductService);
export { ProductService };
//# sourceMappingURL=product.service.js.map