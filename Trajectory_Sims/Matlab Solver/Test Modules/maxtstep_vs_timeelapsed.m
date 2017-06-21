t = [0.1, 0.01, 0.001];
tcompute = [11.5, 58.7, 505]; 
figure(100);
loglog(t,tcompute,'-o'); title('Max Time Step vs Computational Time');
xlabel('Max Time Step (s)'); ylabel('Computational Time (s)'); grid on