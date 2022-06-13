import { Injectable } from '@angular/core';
import { AngularFirestore, AngularFirestoreCollection } from '@angular/fire/firestore';
import { AngularFireAuth } from '@angular/fire/auth';
import { User } from '../interfaces/user';
import { Showers } from '../interfaces/showers';

@Injectable({
  providedIn: 'root'
})
export class AuthService {
  private showersCollection: AngularFirestoreCollection<Showers>;

  constructor(
    private afa: AngularFireAuth,
    private afs: AngularFirestore
  ) { this.showersCollection = this.afs.collection<Showers>('Showers'); }
  
  getShower(id: string) {
    return this.showersCollection.doc<Showers>(id).valueChanges();
  }

  login(user: User) {
    return this.afa.auth.signInWithEmailAndPassword(user.email, user.password);
  }

  register(user: User) {
    return this.afa.auth.createUserWithEmailAndPassword(user.email, user.password);
  }

  logout() {
    return this.afa.auth.signOut();
  }

  getAuth() {
    return this.afa.auth;
  }
}