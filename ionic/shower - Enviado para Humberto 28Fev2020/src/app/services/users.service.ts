import { Injectable } from '@angular/core';
import { AngularFirestore, AngularFirestoreCollection } from '@angular/fire/firestore';
import { Users } from '../interfaces/users';
import { map } from 'rxjs/operators';

@Injectable({
  providedIn: 'root'
})
export class UsersService {
  private usersCollection: AngularFirestoreCollection<Users>;

  constructor(private afs: AngularFirestore) {
    this.usersCollection = this.afs.collection<Users>('Users');
  }

  getUsers() {
    return this.usersCollection.snapshotChanges().pipe(
      map(actions => {
        return actions.map(a => {
          const data = a.payload.doc.data();
          const id = a.payload.doc.id;

          return { id, ...data };
        });
      })
    );
  }

  //addUser(users: Users) {
  //  return this.usersCollection.add(users);
  //}

  addUser(user: Users, timestamp: string) {
    return this.usersCollection.doc<Users>(timestamp).set(user);
  }

  getUser(id: string) {
    return this.usersCollection.doc<Users>(id).valueChanges();
  }
  
  updateUser(id: string, user: Users) {
    return this.usersCollection.doc<Users>(id).update(user);
  }

  deleteUser(id: string) {
    return this.usersCollection.doc(id).delete();
  }
}