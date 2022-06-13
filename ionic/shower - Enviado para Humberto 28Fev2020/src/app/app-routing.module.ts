import { NgModule } from '@angular/core';
import { PreloadAllModules, RouterModule, Routes } from '@angular/router';
import { AuthGuard } from './guards/auth.guard';
import { LoggedGuard } from './guards/logged.guard';

const routes: Routes = [
  { path: '', redirectTo: 'login', pathMatch: 'full' },
  { path: 'home', loadChildren: () => import('./pages/home/home.module').then( m => m.HomePageModule), canActivate: [AuthGuard] },
  { path: 'home/:id', loadChildren: () => import('./pages/home/home.module').then( m => m.HomePageModule), canActivate: [AuthGuard] },
  { path: 'login', loadChildren: () => import('./pages/login/login.module').then( m => m.LoginPageModule), canActivate: [LoggedGuard] },
  { path: 'details', loadChildren: () => import('./pages/details/details.module').then( m => m.DetailsPageModule), canActivate: [AuthGuard] },
  { path: 'details/:id', loadChildren: () => import('./pages/details/details.module').then( m => m.DetailsPageModule), canActivate: [AuthGuard] },
  
];

@NgModule({
  imports: [
    RouterModule.forRoot(routes, { preloadingStrategy: PreloadAllModules })
  ],
  exports: [RouterModule]
})
export class AppRoutingModule { }
