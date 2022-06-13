import { TestBed } from '@angular/core/testing';

import { ShowerService } from './shower.service';

describe('ShowerService', () => {
  beforeEach(() => TestBed.configureTestingModule({}));

  it('should be created', () => {
    const service: ShowerService = TestBed.get(ShowerService);
    expect(service).toBeTruthy();
  });
});
