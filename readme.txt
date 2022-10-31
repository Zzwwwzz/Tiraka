Harjoitustyössä olen käyttänyt seuraavia tietueita:

    struct Solmu
    {
      int indeksi;
      Coord sijainti;

      Solmustatus vari;
      std::shared_ptr<Solmu> edellinen;
      int etaisyys;

      std::unordered_map<Coord, int, CoordHash> naapurit;
    };

    struct Kuitu
    {
      std::shared_ptr<Solmu> solmu1;
      std::shared_ptr<Solmu> solmu2;
      int hinta;
    };

ja solmustatus on määritetly seuraavanlaisesti

enum Solmustatus {White, Gray, Black};

Olen siis tallentanut jokaisen ohjelmassa tarvittavan koordinaatin sekä kahden koordinaatin välissä olevan kuidun omaksi tietueeksi. Graafin solmuja kuvaa koordinaatit ja graafin kaaria kuvaan niiden välille laitettavat valokuidut.

Solmu -tietueihin olen tallentanut itse solmun koordinaatin sekä unordered_map jokaiseen solmuun, mihin kyseisestä solmusta pääsee. Mapissa avaimena on lähtevän solmun koordinaatit sekä datana kaaren paino. Alkiomäärät jäävät tässä säiliössä melko pieneksi, ja ohjelmassa on tarpeellista pystä hakemaan tietoa koordinaattien perusteella, joten päädyin assosatiiviseen säiliöön. Muut tiedot ovat toteutettu mahdollistamaan käytetyt graafialgoritmit. Jokaisen solmun sisälle olen tallentanut sen indeksin, jota käytän vectorin indeksoimiseen Kruskalin algoritmissa. Osoitin edelliseen tallentaa graafialgoritmeissa polun loppupisteestä alkupisteeseen. Vari pitää ylllä solmun statusta graafialgoritmeissa. Etaisyys kuvaa hintaa aloitussolmusta nykyiseen solmuun, ja sillä voi vaihtoehtoisesti kuvata solmujen määrää aloituspisteestä kyseiseen solmuun.
Solmu -olioihin olisi voinut myös lisätä säiliön, jossa olisi tallessa kaikki lähtevät valokuidut järjestyksessä. Mutta koska solmuista lähtevien valokuitujen määrä on niin pieni, en kokenut tarpeelliseksi tehdä koodista entistä vaikealukuisempaa.
Jokaiseen kuitu -tietueehen on tallennettu osoittimet molempiin koordinaattisomuihin, sekä kyseisen kaaren paino.

Naapurit mappiin olisi voinut lisätä reunan painon lisäksi osoittimen haluttuun solmuun. Tämän kuitenkin huomasin olevan paljon epätehokkaampaa kuin tallentaa pelkkä sijainti ja hakea tällä Solmu olio olemassa olevasta map säiliöstä.

Itse ohjelman toteutukseen olen käyttänyt seuraavia tietorakenteita:

	std::vector<std::shared_ptr<Kuitu>> kuidut{};
	std::unordered_map<Coord, std::shared_ptr<Solmu>, CoordHash> solmu_sijainti{};
	std::unordered_map<int, std::shared_ptr<Solmu>> solmu_indeksi;

Jonka lisäksi olen tallentanut tietoja valmiiksi järjestetyistä säiliöistä:

    std::vector<Coord> sorted_xpoints = {};
    std::vector<std::pair<Coord, Coord>> sorted_fibres = {};
    std::vector<std::shared_ptr<Kuitu>> sorted_weight = {};

Tehtäväantoa lukiessa koin, että ohjelman graafi olisi järkevämpää toteuttaa kytkentälistan avulla. Kytkentämatriisi on melko intuitiivinen käyttää, tehtävät poistot ovat vakioaikaisia, sekä tärkeimpänä kaaren olemassaolo/hinta löytyy vakioajassa kahden koordinaatin välille. Kytkentämatriisin huonoja puolia ovat muistinkäyttö sekä hidas solmun lisääminen. Hitaan solmun etsimisen takia toteutin jokaiseen solmuun unordered_map säiliön, josta pystyn löytämään kaikki lähtevät kaaret sekä niiden hinnan. Vaikka datamäärät ovat suuria, niin itse labyrintit eivät ole kovin tiheitä, joten kytkentämatriisi ei ole kovin hyvä vaihtoehto.

Harjoitustyön tehtävänannon vaadittavia komentoja lukiessa huomataan, että ohjelmassa täytyy pystyä tulostamaan kaikki solmusta lähtevät kuidut, kaikki kuidut, sekä kaikki kuitujen päässä oleva koordinaatti. Kaikki kaarien päissä olevat koordinaatit voitaisiin toteuttaa ilman kuidut -vectoria, mutta päätin toteuttaa tietorakenteen lopussa käytettävän Kruskalin algoritmin takia. Tämä vector myös helpottaa ja nopeuttaa ohjelmaa useammassa komennossa. Lisätyt kuidut -vectori mahdollistaa mergesortin. Tähän säiliöön siis lisätään kaikki ei-järjestyksessä olevat kuidut. Kaikki kuidut sisältävää kuidut -vectoria ei tosin pidetä järjestyksessä, vaan olen tehnyt erillisen vectorin luokkaan, joka pitää yllä tällä hetkellä kaikki järjestyksessä olevia valokuituja.

Annetun koodipohjan funktiot toimivat solmun sijainnin mukaan. Tämän takia päädyin toteuttamaan  unordered_mapin, jonka avaimena on sijainti ja datana kaikki solmun tieto. Itse solmujen suuruusjärjestyksellä ei ole ohjelmassa väliä, joten valitsin unordered version säiliöstä.

Mergesortin toteuttaminen all_fibres komentoon nopeuttaa hieman funktiota, mutta monimutkaistaa koodin lukemista. En kokenut, että mergesortin toteutus all_xpoints -funktioon on tarpeellista, koska solmut saadaan epäsuorasti kaarista. Myös fibres -komentoon mahdollinen "järjestetty" bool arvo on mahdollinen, mutta solmun naapureita on todennäköisesti niin vähän, että saavutettu tehokkuus on hyvin minimaalista ja koodista tulisi paljon vaikealukuisempaa. Kaikkiin muihin sorttia käytettäviin kohtiin olen lisännyt tiedon siitä, että onko ohjelmassa tallessa jo valmiiksi järjestyksessä oleva tarvittava säiliö.

Koska harjoitustyön aiheena oli graafialgoritmit, käyn vielä läpi algoritmien valintaperusteet. Ensimmäinen toiminnallisuus on löytää kahden pisteen väliltä reitti. Tähän soveltuu syvyyteen ensin haku (DFS) hyvin, koska meidän ei tarvitse tietää lyhintä mahdollista reittiä. Seuraavaksi täytyi toteuttaa komento, joka löytää labyrintistä ulos reittiä, missä on mahdollisimman vähän risteyksiä. Tähän soveltuu taas leveyteen ensin haku (BFS). Nopeimman reitin löytämiseen voidaan taas valita Dijkstan ja A* väliltä. Labyrinttien kuitujen painon mielivaltaisuus poistaa tosin A* mahdollisuuden, eli vaihtoehdoksi vain Dijkstran algoritmi. Tällöin siis ei lisätä mitään erillistä heurestiikkaa seuraavan solmun tutkimisen valinnassa. Seuraava toiminnallisuus oli löytää labyrintistä silmukka. Tämä onnistuu jo edellä mainitun syvyys ensin haun (DFS) avulla helposti. Tarvitaan vain tutkia onko tutkittavan solmun väri harmaa. Lopuksi haluttiin luoda funktio, joka jättää jäljelle kevyimmän virittävän puun. Vaihtoehtoina mietin Primin algoritmin ja Kruskalin algoritmin väliltä. Päädyin kuitenkin toteuttamaan Kruskalin algoritmin.

Kun haetaan sykli graafista, voitaisiin tallentaa tieto kyseisestä reitistä solmun tietuehen. Mutta koska solmuja on tietokannassa hyvin paljon, saatu hyöty on minimaalista ja ratkaisu veisi hyvin paljon muistia. Toinen epätehokkuus saattaa löytyä Kruskalin algoritmissa kun yhdistetään kaksi puuta. Nykyisessä tietorakenteessa en pysty muuttamaan pienemmän puun lapsien juurta, vaan se osoittaa alkuperäiseen juureen, joka osoittaa isomman puun juureen. Huonossa tapauksessa tietorakenteeseen saattaa syntyä pitkäkin jono. Tehokkuuden kannalta koin kuitenkin, että puun läpikäyminen jokaisessa yhdistyksessä saattaa olla epätehokkaampaa kuin nykyinen ratkaisu.

Harjoitustyön toinen puolisko oli mielestäni mielenkiintoisempi kuin ensimmäinen. Ongelmia tuli hieman osoittimien kanssa, jonka takia käytinkin smart_ptr osoittimia. Tämä hidastaa ohjelmaa hiemna, mutta asymptoottisesti ohjelman pitäisi silti olla melko tehokas.

