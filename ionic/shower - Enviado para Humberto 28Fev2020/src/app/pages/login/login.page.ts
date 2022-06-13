import { Component, OnInit, ViewChild } from '@angular/core';
import { IonSlides, LoadingController, ToastController } from '@ionic/angular';
import { Keyboard } from '@ionic-native/keyboard/ngx';
import { User } from 'src/app/interfaces/user';
import { AuthService } from 'src/app/services/auth.service';
import { Showers } from 'src/app/interfaces/showers'
import { ShowerService } from 'src/app/services/shower.service';
import { Subscription } from 'rxjs';
import { AlertController } from '@ionic/angular';
import { CompileShallowModuleMetadata } from '@angular/compiler';

@Component({
  selector: 'app-login',
  templateUrl: './login.page.html',
  styleUrls: ['./login.page.scss'],
})
export class LoginPage implements OnInit {
  @ViewChild(IonSlides, {static: false}) slides: IonSlides;
  public wavesPosition: number = 0;
  public wavesDifference: number = 80;
  public userLogin: User = {};
  public userRegister: User = {};
  public shower: Showers = {};
  public htmlFieldShowerUID: string;
  //public htmlFieldShowerUIDReady: boolean = false;
  //public showerUID: string;
  //public showers = new Array<Showers>();
  private loading: any;
  private showerSubscription: Subscription;
  //public exists: boolean = false;
  public userID: string;

  constructor(
    public keyboard: Keyboard,
    private loadingCtrl: LoadingController,
    private toastCtrl: ToastController,
    private authService: AuthService,
    private alertCtrl: AlertController,
    private showerService: ShowerService
    ) { }

  ngOnInit() { }

  ngOnDestroy() { 
    if(this.showerSubscription) this.showerSubscription.unsubscribe();
  }

  loadShower(id: string) {
    this.showerSubscription = this.showerService.getShower(id).subscribe(data => {
      this.shower = data;
    });
  }
   
   async showerValidation() {
    if(this.htmlFieldShowerUID) {
      this.confirmValidation('É necessário que o chuveiro esteja OnLine para que se proceda com o cadastro de usuários');
      this.loadShower(this.htmlFieldShowerUID);
    } else {
      this.presentToast("Preencha todos os campos...")
    }
  }

  async confirmValidation(message: string) {
    const alert = await this.alertCtrl.create({
      header: 'Atenção!',
      message: message,
      buttons: [
       {
          text: 'Entendi',
          handler: () => {
            console.log('Confirm Okay');
            this.loadShower(this.htmlFieldShowerUID);
            if(this.shower == undefined){
              this.presentToast("ShowerUID inexistente!");
            } else {
              if(this.shower.showerID == this.htmlFieldShowerUID) {
                this.register();
              }
            }
          }
        }  
      ]
    });
    await alert.present();
  }

  async updateShower() {
    this.userID = this.authService.getAuth().currentUser.uid;
    
      this.shower.userID = this.userID;
      try {
        await this.showerService.updateShower(this.htmlFieldShowerUID, this.shower);
      } catch (error) {
        console.error(error);
        this.presentToast(error);
      }
    
  }

  segmentChanged(event: any) {
    if(event.detail.value === "login") {
      this.slides.slidePrev();
      this.wavesPosition += this.wavesDifference;
    } else {
        this.slides.slideNext();
        this.wavesPosition -= this.wavesDifference;
    }

  }

  async login(){
    await this.presentLoading();
    try {
      await this.authService.login(this.userLogin);
    } catch (error) {
      console.error(error);
      this.presentToast(error.message); 
    } finally {
      this.loading.dismiss();
    }
    
  } 

  async register(){
    await this.presentLoading();
    try {
      await this.authService.register(this.userRegister);
    } catch (error) {
      console.error(error);
      this.presentToast(error.message); 
    } finally {
      this.loading.dismiss();
      this.updateShower();
      //this.showerSubscription.unsubscribe();
    }
    
  } 

  async presentLoading() {
    this.loading = await this.loadingCtrl.create({
      message: 'Aguarde...',
    });
    return this.loading.present();
  }

  async presentToast(message: string) {
    const toast = await this.toastCtrl.create({message, duration: 2000});
    toast.present();
  }

  

}
