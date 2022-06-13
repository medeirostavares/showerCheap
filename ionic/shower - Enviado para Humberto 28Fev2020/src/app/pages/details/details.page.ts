import { Component, OnInit } from '@angular/core';
import { UsersService } from 'src/app/services/users.service';
import { ActivatedRoute } from '@angular/router';
import { Users } from 'src/app/interfaces/users';
import { NavController, LoadingController, ToastController } from '@ionic/angular';
import { AuthService } from 'src/app/services/auth.service';
import { Subscription } from 'rxjs';
import { AlertController } from '@ionic/angular';

@Component({
  selector: 'app-details',
  templateUrl: './details.page.html',
  styleUrls: ['./details.page.scss'],
})
export class DetailsPage implements OnInit {
  editModeSave = true;
  editModeGetUID = false;
  private userAuthId: string = null;
  private userId: string = null;
  private createdNow: number = 0;
  public user: Users = {}; 
  private loading: any;
  private userSubscription: Subscription;

  constructor(
    private usersService: UsersService,
    private activatedRoute: ActivatedRoute,
    private navCtrl: NavController,
    private loadingCtrl: LoadingController,
    private authService: AuthService,
    private toastCtrl: ToastController,
    private alertCtrl: AlertController
  ) {
    this.userId = this.activatedRoute.snapshot.params['id'];

    if (this.userId) this.loadUser(this.userId);
  }

  ngOnInit() { }

  ngOnDestroy() {
    if (this.userSubscription) this.userSubscription.unsubscribe();
  }

  loadUser(userId: string) {
    this.userSubscription = this.usersService.getUser(userId).subscribe(data => {
      this.user = data;
    });
    this.editModeGetUID = true;
  }

  async saveUser() {
    await this.presentLoading();

    this.user.userId = this.authService.getAuth().currentUser.uid;

    if (this.userId) {
      try {
        await this.usersService.updateUser(this.userId, this.user);
        await this.loading.dismiss();
        this.presentToast('Alterações salvas!');
        this.editModeSave = false;
      } catch (error) {
        console.error(error);
        this.presentToast(error);
        this.loading.dismiss();
      }
    } else {
      this.user.createdAt = new Date().getTime();
      this.user.averBathTime = '---';
      this.user.averPowerCons = '---';
      this.user.averWaterCons = '---';
      this.user.tagUID = 'não vinculada';
      this.user.showerID = this.user.userId;
      this.createdNow = this.user.createdAt;
      if(!this.user.name){
        this.loading.dismiss();
        this.presentToast('Preencha o nome do usuário');
      } else {
        try {
          await this.usersService.addUser(this.user, this.createdNow.toString());
          await this.loading.dismiss();
          this.presentToast('Usuário Salvo! Favor vincular TagUID');
          this.user = {};
          var newID = this.createdNow.toString();  
          this.loadUser(newID);
          this.editModeSave = false;
          this.editModeGetUID = true;
        } catch (error) {
          console.error(error);
          this.presentToast(error);
          this.loading.dismiss();
        }
      }
    }
  }

  getTagUID() {
      this.tagRFIDConfirm('Aproxime a Tag do leitor RFID...');
  }

  presentLoadingMessage(message: string) {
    this.loading = this.loadingCtrl.create({ message, duration: 5000 });
    return this.loading.present();
  }

  async presentLoading() {
    this.loading = await this.loadingCtrl.create({ message: 'Aguarde...' });
    return this.loading.present();
  }

  async presentToast(message: string) {
    const toast = await this.toastCtrl.create({ message, duration: 3500 });
    toast.present();
  }

  async tagRFIDConfirm(message: string) {
    const alert = await this.alertCtrl.create({
      header: 'Vincular tagRFID ao usuário',
      message: message,
      buttons: [
        {
          text: 'Cancelar',
          role: 'cancel',
          cssClass: 'secondary',
          handler: (blah) => {
            console.log('Confirm Cancel');
          }
        }, {
          text: 'Pronto',
          handler: () => {
            console.log('Confirm Okay');
            if(this.createdNow != 0) {
              var newID = this.createdNow.toString();  
              this.loadUser(newID);
              console.info("Com novo ID: ", newID);
            } else {
                this.loadUser(this.userId);
                console.info("Com user ID: ", this.user.userId);
            }
      
            if(this.user.tagUID != 'não vinculada') {
              this.presentToast('TagUID vinculada com sucesso!');
              this.navCtrl.navigateBack('/home');
              console.info(this.user);
            } else {
              console.info(this.user);
              this.presentToast('Tag não detectada, tente novamente...');
      }
          }
        }
      ]
    });

    await alert.present();
  }

  
}