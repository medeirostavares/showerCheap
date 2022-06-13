import { Injectable } from '@angular/core';
import { AngularFirestore, AngularFirestoreCollection } from '@angular/fire/firestore';
import { Showers } from '../interfaces/showers';
import { map } from 'rxjs/operators';

@Injectable({
  providedIn: 'root'
})

export class ShowerService {
  private showersCollection: AngularFirestoreCollection<Showers>;

  constructor(
    private afs: AngularFirestore
  ) { 
    this.showersCollection = this.afs.collection<Showers>('Showers'); 
    }
  

  selectShower() {
    //return this.usersCollection()
  }

  getShowers() {
    return this.showersCollection.snapshotChanges().pipe(
      map(actions => {
        return actions.map(a => {
          const data = a.payload.doc.data();
          const id = a.payload.doc.id;

          return { id, ...data };
        });
      })
    );
  }

  getShower(id: string) {
    return this.showersCollection.doc<Showers>(id).valueChanges();
  }

  updateShower(id: string, shower: Showers){
    return this.showersCollection.doc<Showers>(id).update(shower);
  }

}