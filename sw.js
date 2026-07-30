/* M20C Fuel Timer — offline service worker
   Caches the app on first visit and serves it from the device forever after.
   Bump CACHE_VERSION whenever index.html changes so phones pick up the update. */

const CACHE_VERSION = 'm20c-fuel-timer-v4';

const FILES = [
  './',
  './index.html'
];

// Install: pull the app into the cache immediately.
self.addEventListener('install', (event) => {
  event.waitUntil(
    caches.open(CACHE_VERSION)
      .then((cache) => cache.addAll(FILES))
      .then(() => self.skipWaiting())
  );
});

// Activate: drop any older caches so we don't accumulate stale copies.
self.addEventListener('activate', (event) => {
  event.waitUntil(
    caches.keys()
      .then((keys) => Promise.all(
        keys.filter((k) => k !== CACHE_VERSION).map((k) => caches.delete(k))
      ))
      .then(() => self.clients.claim())
  );
});

// Fetch: cache first. The app never needs the network, so this always wins.
self.addEventListener('fetch', (event) => {
  if (event.request.method !== 'GET') return;

  event.respondWith(
    caches.match(event.request).then((hit) => {
      if (hit) return hit;

      return fetch(event.request)
        .then((res) => {
          // Stash a copy of anything new we successfully fetch.
          if (res && res.status === 200 && res.type === 'basic') {
            const copy = res.clone();
            caches.open(CACHE_VERSION).then((c) => c.put(event.request, copy));
          }
          return res;
        })
        .catch(() => {
          // Offline and not cached: fall back to the app shell for page loads.
          if (event.request.mode === 'navigate') {
            return caches.match('./index.html');
          }
        });
    })
  );
});
