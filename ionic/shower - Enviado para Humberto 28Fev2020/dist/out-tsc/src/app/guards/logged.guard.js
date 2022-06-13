import * as tslib_1 from "tslib";
import { Injectable } from '@angular/core';
import { Router } from '@angular/router';
import { AuthService } from '../services/auth.service';
let LoggedGuard = class LoggedGuard {
    constructor(authService, router) {
        this.authService = authService;
        this.router = router;
    }
    canActivate() {
        return new Promise(resolve => {
            this.authService.getAuth().onAuthStateChanged(user => {
                if (user)
                    this.router.navigate(['home']);
                resolve(!user ? true : false);
            });
        });
    }
};
LoggedGuard = tslib_1.__decorate([
    Injectable({
        providedIn: 'root'
    }),
    tslib_1.__metadata("design:paramtypes", [AuthService,
        Router])
], LoggedGuard);
export { LoggedGuard };
//# sourceMappingURL=logged.guard.js.map