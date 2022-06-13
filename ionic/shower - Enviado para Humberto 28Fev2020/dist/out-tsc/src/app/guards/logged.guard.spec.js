import { TestBed, inject } from '@angular/core/testing';
import { LoggedGuard } from './logged.guard';
describe('LoggedGuard', () => {
    beforeEach(() => {
        TestBed.configureTestingModule({
            providers: [LoggedGuard]
        });
    });
    it('should ...', inject([LoggedGuard], (guard) => {
        expect(guard).toBeTruthy();
    }));
});
//# sourceMappingURL=logged.guard.spec.js.map