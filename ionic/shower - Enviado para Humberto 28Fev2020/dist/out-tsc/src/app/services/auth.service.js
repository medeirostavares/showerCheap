import * as tslib_1 from "tslib";
import { Injectable } from '@angular/core';
import { AngularFireAuth } from '@angular/fire/auth';
let AuthService = class AuthService {
    constructor(afa) {
        this.afa = afa;
    }
    login(user) {
        return this.afa.auth.signInWithEmailAndPassword(user.email, user.password);
    }
    register(user) {
        return this.afa.auth.createUserWithEmailAndPassword(user.email, user.password);
    }
    logout() {
        return this.afa.auth.signOut();
    }
    getAuth() {
        return this.afa.auth;
    }
};
AuthService = tslib_1.__decorate([
    Injectable({
        providedIn: 'root'
    }),
    tslib_1.__metadata("design:paramtypes", [AngularFireAuth])
], AuthService);
export { AuthService };
//# sourceMappingURL=auth.service.js.map