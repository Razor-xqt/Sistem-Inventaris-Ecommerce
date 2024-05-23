# Sistem Manajemen Toko Online

---

#### **Deskripsi Projek**

Aplikasi Manajemen Toko Online untuk pengelolaan produk, pelanggan, dan pesanan. Aplikasi ini akan memanfaatkan semua struktur data dan algoritma serta berbagai fungsi yang efisien dari segi waktu (*Time complexity*) dan ruang (*Space Compexity* atau Manajemen memory).

---

#### **Menu Manajemen Toko Online**

1. Dashboard Produk
2. Dashboard Pelanggan
3. Dashboard Transaksi

---

#### ~ Rincian Dashboard ~

---

##### 1. Dashboard Produk
1. Tambah Produk 
2. Update Produk
3. Hapus Produk
4. Cari Produk
5. Tampilkan dan Sorting Produk 

##### 2. Dashboard Pelanggan
1. Tambah Pelanggan
2. Update Pelanggan
3. Hapus Pelanggan
4. Cari Pelanggan
5. Tampilkan dan Sorting Pelanggan


##### 3. Dashboard Transaksi
1. Pembelian Produk oleh pelanggan
2. Antrian Pesanan Pelanggan (verifikasi)
3. Riwayat Pesanan Pelanggan
4. Produk yang sering dibeli oleh pelanggan

---

#### ~ Rincian detail ~

---

#### Dashboard Produk

##### 1. Tambah Produk 

> Implementasi penambahan data **linked list**.
> Data akan ditambahkan dalam **linked list**.

Produk yang ditambahkan berisi data produk mencakup:
- Nama produk
- Id Produk (*khusus id produk di generate menggunakan rekursif*)
- Stock produk
- Kategori produk
  
> Implementasi penambahan data pada **tree**.
> Data Nama produk, id produk, stock produk dan kategori produk juga akan disimpan kedalam **tree** yang dimana kategori produk sebagai parent dan nama, id, stock produk sebagai child.

##### 2. Update Produk

> Implementasi perubahan data **linked list**
> Akan dilakukan perubahan data pada **linked list**.

Opsi ini untuk mengupdate data produk mencakup menggunakan id produk:
- Nama produk
- Stock produk
- Kategori produk

> Implementasi perubahan data pada **tree.**
> 
> Data Nama produk, id produk, stock produk juga akan diubah didalam tree yang dimana kategori produk sebagai parent dan nama, id, stock produk sebagai child.
 
##### 3. Hapus Produk

> Implementasi penghapusan data pada **linked list**,
> Produk akan dihapus yang tersimpan dalam **linked list**.
> 
> Implementasi Penghapusan data Pada **tree**,
> Produk akan dihapus yang tersimpan dalam **tree**.

Opsi ini untuk menghapus produk dengan id produk dan menampilkan detail produk yang dihapus.

##### 4. Cari Produk

> Implementasi Pencarian data **Linked list** menggunakan **Algoritma Searching**

Opsi ini akan mencari produk berdasakan:
- Nama Produk
- Id produk

Dan menampilkan detail produk yang dicari.

##### 5. Kategori Produk

> Implementasi **tree**,
> Data pada **tree** akan ditampilkan yang dimana kategori sebagai parents dan nama, id, dan stock produk sebagai child.

Opsi ini akan menampilkan nama produk, id produk, stock produk yang tersedia berdasarkan kategori masing-masing produk.

#####  6. Tampilkan dan Sorting Semua Produk

> Implementasi Sorting data **Linked List** menggunakan **Algoritma Sorting**

Opsi ini akan mensorting semua produk berdasarkan 'Nama Produk' secara Ascending (A-Z) dan menampilkan semua produk yang dilengkapi dengan detail produk yang telah disorting.

---

#### Dashboard Pelanggan

##### 1. Tambah Pelanggan

> Implementasi penambahan data pada **Hash Table** dan **Collision Handling**

Opsi ini untuk menambah pelanggan yang berisi data pelanggan mencakup:
- Nama Pelanggan
- ID pelanggan (*khusus id pelanggan akan tergenerate secara rekursif*)
- Alamat Pelanggan
- No. Telephone Pelanggan

Dan menampilkan detail pelanggan yang ditambahkan.

##### 2. Update Pelanggan

> Implementasi perubahan data pada **Hash Table** dan **Collision Handling**

Opsi ini untuk mengupdate data pelanggan mencakup:
- Nama Pelanggan
- ID Pelanggan
- Alamat Pelanggan
- No. Telephone Pelanggan

Dan menampilkan detail pelanggan yang diubah.

##### 3. Hapus Pelanggan

> Implementasi penghapusan data  pada **Hash Table** dan **Collision Handling**

Opsi ini untuk menghapus pelanggan berdasarkan ID pelanggan dan menampilkan detail pelanggan yang dihapus dan menampilkan detail pelanggan yang dihapus.

##### 4. Cari Pelanggan

> Implementasi Pencarian cepat Hash Table menggunakan **Algoritma Searching**

Opsi ini akan mencari data pelanggan mencakup:
- Nama Pelanggan
- Id Pelanggan

Dan menampilkan detail data pelanggan yang dicari.

##### 5. Tampilkan dan Sorting pelanggan

> Implementasi Sorting data **Hash Table** menggunakan **Algoritma Sorting**

Opsi ini akan mensorting data pelanggan berdasarkan nama pelanggan secara Ascending (A-z) dan menampilkan semua data pelanggan yang telah disorting.

---

#### Dashboard Transaksi

##### 1. Pembelian/ pemesanan  produk oleh pelanggan

> Implementasi **Queue**

Opsi ini untuk membeli produk oleh pelanggan yang terdaftar dan akan dilakukan verifikasi dibagian verifikasi Pesanan pelanggan.
- Pembelian memerlukan data pelanggan yaitu ID
- Pembelian memerlukan Id produk yang dibeli

> Pembelian akan mengurangi stock produk yang ada didalam **linkedlist** dan **tree** hingga bernilai 0. ketika bernilai 0 maka tidak bisa dilakukan pemesanan.
> 
> Pembelian juga akan dicatat di menu 'produk yang sering dibeli oleh pelanggan (yang tersimpan dalam **graph**)' untuk produk yang sering dibeli.

##### 2. Antrian Pesanan Pelanggan (Verifikasi)

> implementasi **Queue**

Opsi ini berupa antrian pesanan produk pelanggan yang akan disetujui atau tidak disetujui. Setelah pesanan disetujui akan di catat di bagian riwayat pemesanan produk.

> Antrian ini disimpan dalam **queue** secara first in first out.
> Setelah pesanan disetujui akan dipindahkan ke **stack** pada bagian riwayat pesanan produk.

##### 3. Riwayat Pesanan Produk

> Implementasi **Stack**,
> Menampilkan riwayat pembelian secara **stack** (first in last out)

Opsi ini untuk menampilkan riwayat pembelian/pemesanan produk setelah dilakukan verifikasi pesanan.

##### 4.  Produk yang sering dibeli oleh pelanggan.

> Implementasi **Graph**

Opsi ini menampilkan produk yang sering dibeli oleh pelanggan dan menampilkan detail produk serta berapa banyak produk yang dibeli.

