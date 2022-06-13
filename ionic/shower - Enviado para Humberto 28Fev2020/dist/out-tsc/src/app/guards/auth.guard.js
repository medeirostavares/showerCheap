import * as tslib_1 from "tslib";
import { Injectable } from '@angular/core';
import { Router } from '@angular/router';
import { AuthService } from '../services/auth.service';
let AuthGuard = class AuthGuard {
    constructor(authService, router) {
        this.authService = authService;
        this.router = router;
    }
    canActivate() {
        return new Promise(resolve => {
            this.authService.getAuth().onAuthStateChanged(user => {
                if (!user)
                    this.router.navigate(['login']);
                resolve(user ? true : false);
            });
        });
    }
};
AuthGuard = tslib_1.__decorate([
    Injectable({
        providedIn: 'root'
    }),
    tslib_1.__metadata("design:paramtypes", [AuthService,
        Router])
], AuthGuard);
export { AuthGuard };
//# sourceMappingURL=auth.guard.js.map