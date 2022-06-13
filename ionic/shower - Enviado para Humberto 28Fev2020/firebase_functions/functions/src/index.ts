//Link de Acesso:
//https://showercheap-9ae49.firebaseapp.com/api/v1/showers/projetomestradouft

import * as functions from 'firebase-functions';
import * as admin from 'firebase-admin';
import * as firebaseHelper from 'firebase-functions-helper';
import * as express from 'express';
import * as bodyParser from 'body-parser';

admin.initializeApp(functions.config().firebase);

const db = admin.firestore();
const app = express(); 
const main = express();

main.use(bodyParser.json());
main.use(bodyParser.urlencoded({extended: false}));
main.use('/api/v1', app);

const showersCollection = 'Showers';

export const esp32Firestore = functions.https.onRequest(main);

app.patch('/showers/:showerId', async(req, res) => {
    try {
        await firebaseHelper.firestore.updateDocument(db, showersCollection, req.params.showerId, req.body);
        res.status(200).send('Update Success!');
    } catch(error) {
        res.status(204).send('Patch Error!');
    }
})