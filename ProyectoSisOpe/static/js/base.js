// Inicialización de animaciones AOS
AOS.init({
    duration: 500,
    once: true
});

const byId = id => document.getElementById(id);

document.addEventListener("DOMContentLoaded", () => {
    const btnToggle = byId('btnToggle');
    const appContainer = byId('appContainer');
    const navItems = document.querySelectorAll('.nav-menu .nav-item');
    const pagePanels = document.querySelectorAll('.page-panel');

    // 1. Manejo del Clic en las 3 líneas (Encoger/Expandir)
    if (btnToggle && appContainer) {
        btnToggle.addEventListener('click', () => {
            appContainer.classList.toggle('collapsed');
        });
    }

    // 2. Manejo del cambio de paneles (Asignación directa a cada botón)
    navItems.forEach(item => {
        item.addEventListener('click', (event) => {
            // Evitamos cualquier comportamiento extraño del navegador
            event.preventDefault();

            // Quitar clase 'active' de todos los botones del menú
            navItems.forEach(nav => nav.classList.remove('active'));
            
            // Quitar clase 'active' de todos los paneles con transición suave
            pagePanels.forEach(panel => {
                panel.classList.remove('active');
            });

            // Activar el botón actual al que le diste clic
            item.classList.add('active');

            // Obtener el ID del panel objetivo y activarlo
            const targetId = item.getAttribute('data-target');
            const targetPanel = byId(targetId);
            
            if (targetPanel) {
                // Pequeño delay para permitir que la transición de opacidad se vea
                setTimeout(() => {
                    targetPanel.classList.add('active');
                    
                    // Forzar el refresco de animaciones AOS
                    targetPanel.classList.remove('aos-animate');
                    setTimeout(() => {
                        targetPanel.classList.add('aos-animate');
                    }, 30);
                }, 50);
            }
        });
    });
});