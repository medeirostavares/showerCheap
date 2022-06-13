import { Component, OnInit } from '@angular/core';
import { AuthService } from 'src/app/services/auth.service';
import { NavController, LoadingController, ToastController, IonApp } from '@ionic/angular';
import { UsersService } from 'src/app/services/users.service';
import { ActivatedRoute } from '@angular/router';
import { ShowerService } from 'src/app/services/shower.service';
import { Users } from 'src/app/interfaces/users';
import { Showers } from 'src/app/interfaces/showers';
import { Subscription } from 'rxjs';

@Component({
  selector: 'app-home',
  templateUrl: './home.page.html',
  styleUrls: ['./home.page.scss'],
})
export class HomePage implements OnInit {
  editMode: boolean = true;
  private loading: any;
  public users = new Array<Users>();
  public showers = new Array<Showers>();
  //public shower: Showers = {}; 
  //private showerUID: string;
  public showerStatusConnect: boolean;
  public loggedUserUID: string;
  private usersSubscription: Subscription;
  private showerSubscription: Subscription;

  constructor(
    private activatedRoute: ActivatedRoute,
    private navCtrl: NavController,
    private authService: AuthService,
    private loadingCtrl: LoadingController,
    private usersService: UsersService,
    private route: ActivatedRoute,
    private showerService: ShowerService,
    private toastCtrl: ToastController
  ) {

    this.usersSubscription = this.usersService.getUsers().subscribe(data => {
      this.users = data;
    });

    this.showerSubscription = this.showerService.getShowers().subscribe(data => {
      this.showers = data;
    });
  }

  ngAfterContentChecked(){
    console.info("ENTROU");
    if((this.showers.length) && (this.showers != undefined)) {
      console.info(this.showers.length);
      this.loadShower();
    }
    
  }

  ngOnInit() { 
    this.loggedUserUID = this.authService.getAuth().currentUser.uid; 
  }

  ngOnDestroy() {
    this.usersSubscription.unsubscribe();
    this.showerSubscription.unsubscribe();
  }

  loadShower() {
    let shower = this.showers.filter((loggedOn) => {
      return loggedOn.userID === this.loggedUserUID;
    }) 
    this.showerStatusConnect = shower[0].showerStatusConnect;
    console.info(this.showerStatusConnect);
  }

  
  showerStatusShow() { 
    if(this.showerStatusConnect == true) {
    return "OnLine";
    }
    else {
      return "OffLine";
    }
  }

  toolBarColor() { 
    if(this.showerStatusConnect == true) {
    return "success";
    }
    else {
      return "danger";
    }
  }
  
  async logout() {
    await this.presentLoading();

    try {
      await this.authService.logout();
    } catch (error) {
      console.error(error);
    } finally {
      this.loading.dismiss();
    }
  }

  async presentLoading() {
    this.loading = await this.loadingCtrl.create({ message: 'Aguarde...' });
    return this.loading.present();
  }

  async deleteUser(id: string) {
    try {
      await this.usersService.deleteUser(id);
    } catch (error) {
      this.presentToast('Erro ao tentar deletar');
    }
  }

  async presentToast(message: string) {
    const toast = await this.toastCtrl.create({ message, duration: 2000 });
    toast.present();
  }
}